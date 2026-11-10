#include "Cursor_Surveillance.h"

bool Cursor_Surveillance::Sample(POINT &position, operational_space &space) const
{
    if (!GetCursorPos(&position))
        return false;
    const auto monitor = MonitorFromPoint(position, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info{};
    info.cbSize = sizeof(info);
    if (!GetMonitorInfoW(monitor, &info))
        return false;
    space = (info.dwFlags & MONITORINFOF_PRIMARY) ? operational_space::Desktop : operational_space::Virtual;
    return true;
}
