#pragma once
#include<Windows.h>
enum class operational_space{ Desktop, Virtual };
class Cursor_Surveillance
{
public:
	Cursor_Surveillance();
	~Cursor_Surveillance();
private:
	unsigned int DesktopWidth = 1920;
	unsigned int DesktopHeight = 1080;
	unsigned int VirtualDesktopWidth = 1920;
	unsigned int VirtualDesktopHeight = 1080;
	POINT VirtualCenter;
	operational_space op_space;
	POINT PrevPoint,Point,InitPoint;
	BOOL CenterInitialized = false;

};

