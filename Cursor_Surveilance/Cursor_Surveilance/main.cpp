#include "Cursor_Surveillance.h"
#include <iostream>
#include <string_view>

int main(int argc, char *argv[])
{
    const std::string_view mode = argc == 2 ? argv[1] : "";
    if (argc != 2 || (mode != "--once" && mode != "--watch"))
    {
        std::cout
            << "cursor-surveillance --once | --watch\nRead-only cursor position; Escape stops watch mode.\n";
        return argc == 1 || mode == "--help" ? 0 : 2;
    }
    Cursor_Surveillance observer;
    do
    {
        POINT point{};
        operational_space space{};
        if (!observer.Sample(point, space))
        {
            std::cerr << "Cannot read cursor monitor.\n";
            return 1;
        }
        std::cout << point.x << ',' << point.y << ' '
                  << (space == operational_space::Desktop ? "primary" : "secondary") << '\n';
        if (mode == "--once")
            break;
        Sleep(100);
    } while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000));
    return 0;
}
