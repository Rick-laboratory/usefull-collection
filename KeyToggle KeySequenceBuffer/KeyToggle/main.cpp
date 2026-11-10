#include "Key_Operator.h"
#include <array>
#include <iostream>
#include <string_view>

class KeySequence
{
  public:
    bool Press(int key)
    {
        constexpr std::array<int, 3> expected{VK_CONTROL, 'R', 'I'};
        if (key == expected[next_])
            ++next_;
        else
            next_ = key == expected[0] ? 1 : 0;
        if (next_ != expected.size())
            return false;
        next_ = 0;
        return true;
    }

  private:
    std::size_t next_ = 0;
};
int main(int argc, char *argv[])
{
    const std::string_view mode = argc == 2 ? argv[1] : "";
    if (mode == "--self-test")
    {
        KeySequence sequence;
        if (sequence.Press('R') || sequence.Press('I') || sequence.Press(VK_CONTROL) || sequence.Press('R') ||
            !sequence.Press('I') || sequence.Press('I'))
            return 1;
        std::cout << "Sequence checks passed\n";
        return 0;
    }
    if (mode != "--watch")
    {
        std::cout << "key-sequence --watch\nDetects Ctrl, R, I press edges in that order. Escape stops.\n";
        return argc == 1 || mode == "--help" ? 0 : 2;
    }
    std::array<Key_Operator, 3> keys{Key_Operator(VK_CONTROL), Key_Operator('R'), Key_Operator('I')};
    constexpr std::array<int, 3> codes{VK_CONTROL, 'R', 'I'};
    KeySequence sequence;
    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
    {
        for (std::size_t i = 0; i < keys.size(); ++i)
            if (keys[i] && sequence.Press(codes[i]))
                std::cout << "Ctrl -> R -> I\n";
        Sleep(8);
    }
    return 0;
}
