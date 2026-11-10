#include <windows.h>
#include <iostream>
#include <string_view>
#include <vector>

int main(int argc, char *argv[])
{
    if (argc != 2 || (std::string_view(argv[1]) != "--record" && std::string_view(argv[1]) != "--replay"))
    {
        std::cout << "cursor-replay --record | --replay\n"
                     "Samples 100 cursor positions at 10 ms intervals. --replay also moves the cursor.\n"
                     "Press Escape to stop.\n";
        return argc == 1 || (argc == 2 && std::string_view(argv[1]) == "--help") ? 0 : 2;
    }
    std::vector<POINT> points;
    points.reserve(100);
    for (unsigned i = 0; i < 100 && !(GetAsyncKeyState(VK_ESCAPE) & 0x8000); ++i)
    {
        POINT point{};
        if (!GetCursorPos(&point))
        {
            std::cerr << "GetCursorPos failed.\n";
            return 1;
        }
        points.push_back(point);
        Sleep(10);
    }
    for (const auto &point : points)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;
        std::cout << point.x << ',' << point.y << '\n';
        if (std::string_view(argv[1]) == "--replay")
        {
            if (!SetCursorPos(point.x, point.y))
            {
                std::cerr << "SetCursorPos failed.\n";
                return 1;
            }
            Sleep(10);
        }
    }
    return 0;
}
