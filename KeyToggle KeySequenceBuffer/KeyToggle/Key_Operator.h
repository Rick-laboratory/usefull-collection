#pragma once
#include <windows.h>

class Key_Operator
{
  public:
    explicit Key_Operator(int key) : key_(key)
    {
    }
    explicit operator bool()
    {
        const bool down = (GetAsyncKeyState(key_) & 0x8000) != 0;
        const bool pressed = down && !active_;
        active_ = down;
        return pressed;
    }

  private:
    int key_;
    bool active_ = false;
};
