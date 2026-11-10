#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "event_queue.hpp"

#include <atomic>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

namespace
{
EventQueue events;
std::atomic<DWORD> message_thread{0};
std::atomic<unsigned> dropped_events{0};
bool include_movement = false; // Set before the hook is installed.

const char *event_name(std::uint32_t message) noexcept
{
    switch (message)
    {
    case WM_MOUSEMOVE:
        return "Move";
    case WM_LBUTTONDOWN:
        return "Left down";
    case WM_LBUTTONUP:
        return "Left up";
    case WM_RBUTTONDOWN:
        return "Right down";
    case WM_RBUTTONUP:
        return "Right up";
    case WM_MBUTTONDOWN:
        return "Middle down";
    case WM_MBUTTONUP:
        return "Middle up";
    case WM_XBUTTONDOWN:
        return "X button down";
    case WM_XBUTTONUP:
        return "X button up";
    case WM_MOUSEWHEEL:
        return "Wheel";
    case WM_MOUSEHWHEEL:
        return "Horizontal wheel";
    default:
        return "Mouse event";
    }
}

LRESULT CALLBACK mouse_callback(int code, WPARAM message, LPARAM data)
{
    if (code == HC_ACTION && data != 0 && (include_movement || message != WM_MOUSEMOVE))
    {
        const auto &mouse = *reinterpret_cast<const MSLLHOOKSTRUCT *>(data);
        const MouseEvent event{static_cast<std::uint32_t>(message), mouse.pt.x, mouse.pt.y, mouse.mouseData};
        if (!events.try_push(event))
            dropped_events.fetch_add(1, std::memory_order_relaxed);
    }
    // Always forward the event: this example observes input, it never suppresses it.
    return CallNextHookEx(nullptr, code, message, data);
}

BOOL WINAPI console_handler(DWORD event)
{
    if (event == CTRL_C_EVENT || event == CTRL_BREAK_EVENT)
    {
        return PostThreadMessageW(message_thread.load(std::memory_order_relaxed), WM_QUIT, 0, 0);
    }
    return FALSE;
}

class ConsoleHandler
{
  public:
    ConsoleHandler()
    {
        if (!SetConsoleCtrlHandler(console_handler, TRUE))
        {
            throw std::runtime_error("SetConsoleCtrlHandler failed.");
        }
    }
    ~ConsoleHandler()
    {
        SetConsoleCtrlHandler(console_handler, FALSE);
    }
    ConsoleHandler(const ConsoleHandler &) = delete;
    ConsoleHandler &operator=(const ConsoleHandler &) = delete;
};

class MouseHook
{
  public:
    MouseHook() : handle_(SetWindowsHookExW(WH_MOUSE_LL, mouse_callback, GetModuleHandleW(nullptr), 0))
    {
        if (handle_ == nullptr)
        {
            throw std::runtime_error("SetWindowsHookEx failed: " + std::to_string(GetLastError()));
        }
    }
    ~MouseHook()
    {
        UnhookWindowsHookEx(handle_);
    }
    MouseHook(const MouseHook &) = delete;
    MouseHook &operator=(const MouseHook &) = delete;

  private:
    HHOOK handle_;
};

class EventLogger
{
  public:
    EventLogger() : worker_([this] { run(); })
    {
    }
    ~EventLogger()
    {
        stopping_.store(true, std::memory_order_release);
        worker_.join();
    }
    EventLogger(const EventLogger &) = delete;
    EventLogger &operator=(const EventLogger &) = delete;

  private:
    void run()
    {
        while (true)
        {
            MouseEvent event;
            if (events.pop(event))
            {
                std::cout << event_name(event.message) << "  x=" << event.x << " y=" << event.y;
                if (event.message == WM_MOUSEWHEEL || event.message == WM_MOUSEHWHEEL)
                {
                    std::cout << " delta=" << static_cast<short>(HIWORD(event.data));
                }
                else if (event.message == WM_XBUTTONDOWN || event.message == WM_XBUTTONUP)
                {
                    std::cout << " button=" << HIWORD(event.data);
                }
                std::cout << std::endl;
            }
            else if (stopping_.load(std::memory_order_acquire))
            {
                return;
            }
            else
            {
                Sleep(10);
            }
        }
    }

    std::atomic<bool> stopping_{false};
    std::thread worker_;
};
} // namespace

int main(int argc, char *argv[])
{
    const std::string_view option = argc == 2 ? argv[1] : "";
    if (argc == 2 && (option == "--help" || option == "-h"))
    {
        std::cout << "Usage: mouse-hook [--moves | --check | --help]\n"
                     "Observe mouse buttons and wheel events; stop with Ctrl+C.\n"
                     "--moves also logs motion. --check installs/removes the hook and exits.\n";
        return 0;
    }
    if (argc > 2 || (argc == 2 && option != "--moves" && option != "--check"))
    {
        std::cerr << "Usage: mouse-hook [--moves | --check | --help]\n";
        return 2;
    }

    try
    {
        include_movement = option == "--moves";
        message_thread.store(GetCurrentThreadId(), std::memory_order_relaxed);
        MSG message{};
        PeekMessageW(&message, nullptr, WM_USER, WM_USER, PM_NOREMOVE); // Create the queue.
        const ConsoleHandler handler;

        if (option == "--check")
        {
            const MouseHook hook;
            std::cout << "Mouse hook installed successfully; removing it on exit.\n";
            return 0;
        }

        std::cout << "Observing mouse input. Press Ctrl+C to stop.\n" << std::flush;
        int result = 0;
        {
            EventLogger logger;
            const MouseHook hook; // Unhook before the logger drains and joins.
            BOOL received = 0;
            while ((received = GetMessageW(&message, nullptr, 0, 0)) > 0)
            {
                TranslateMessage(&message);
                DispatchMessageW(&message);
            }
            if (received == -1)
            {
                std::cerr << "GetMessage failed: " << GetLastError() << '\n';
                result = 1;
            }
        }
        std::cout << "Stopped. Dropped events: " << dropped_events.load() << '\n';
        return result;
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
