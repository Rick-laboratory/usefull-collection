#include "Cursor_Surveillance.h"
#include<iostream>

Cursor_Surveillance::Cursor_Surveillance()
{
	VirtualCenter.x = DesktopWidth + VirtualDesktopWidth / 2;
	VirtualCenter.y = DesktopHeight / 2;
	while (true)
	{
		if (CenterInitialized) {

		}
		else
		{
			GetCursorPos(&Point);
		}
		if (Point.x < 1920 && Point.y < 1080)
		{
			op_space = operational_space::Desktop;
			std::cout << "desktop X:" << Point.x << " Y:" << Point.y << "\n";
		}
		else
		{
			op_space = operational_space::Virtual;
			std::cout << "virtual X:"<< Point.x << " Y:" << Point.y<<"\n";
			SetCursorPos(VirtualCenter.x, VirtualCenter.y);
			CenterInitialized = true;
		}
	}
}


Cursor_Surveillance::~Cursor_Surveillance()
{
}
