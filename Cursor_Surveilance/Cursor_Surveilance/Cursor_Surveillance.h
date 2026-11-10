#pragma once
#include <windows.h>

enum class operational_space
{
    Desktop,
    Virtual
};
class Cursor_Surveillance
{
  public:
    // Reads a position and classifies its actual monitor; it never moves the pointer.
    bool Sample(POINT &position, operational_space &space) const;
};
