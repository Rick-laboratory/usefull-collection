#include <windows.h>
#include <atomic>
#include <iostream>
#include <string_view>
#include <vector>

namespace
{
std::atomic_bool stopping{false};
BOOL WINAPI stop(DWORD event)
{
    if (event != CTRL_C_EVENT && event != CTRL_BREAK_EVENT)
        return FALSE;
    stopping.store(true);
    return TRUE;
}
struct RestoreCursor
{
    ~RestoreCursor()
    {
        SystemParametersInfoW(SPI_SETCURSORS, 0, nullptr, 0);
    }
};
} // namespace
int main(int argc, char *argv[])
{
    if (argc != 2 || std::string_view(argv[1]) != "--hide")
    {
        std::cout << "hide-cursor --hide\nTemporarily replaces the system arrow cursor for 15 seconds.\n"
                     "Ctrl+C restores early. Forced process termination can prevent cleanup.\n";
        return argc == 1 || (argc == 2 && std::string_view(argv[1]) == "--help") ? 0 : 2;
    }
    const int width = GetSystemMetrics(SM_CXCURSOR), height = GetSystemMetrics(SM_CYCURSOR);
    if (width <= 0 || height <= 0 || width > 1024 || height > 1024)
        return 1;
    const auto bytes = static_cast<std::size_t>(((width + 15) / 16) * 2) * height;
    const std::vector<unsigned char> andMask(bytes, 0xff), xorMask(bytes, 0);
    HCURSOR cursor =
        CreateCursor(GetModuleHandleW(nullptr), 0, 0, width, height, andMask.data(), xorMask.data());
    if (!cursor)
        return 1;
    if (!SetConsoleCtrlHandler(stop, TRUE))
    {
        DestroyCursor(cursor);
        return 1;
    }
    RestoreCursor restore;
    // SetSystemCursor takes ownership of the supplied cursor.
    if (!SetSystemCursor(cursor, 32512))
    {
        SetConsoleCtrlHandler(stop, FALSE);
        return 1;
    }
    for (int tick = 0; tick < 300 && !stopping.load(); ++tick)
        Sleep(50);
    SetConsoleCtrlHandler(stop, FALSE);
    return 0;
}
