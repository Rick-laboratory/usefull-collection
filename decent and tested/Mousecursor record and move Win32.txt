#include<Windows.h>
#include<iostream>
#include<vector>
int main(void) {
	POINT position;


	std::vector<POINT> points;

	for (size_t i = 0; i < 100; i++)
	{
		if(GetCursorPos(&position))
			points.push_back(position);

		Sleep(1);

	}
	for (size_t i = 0; i < points.size(); i++)
	{
		SetCursorPos(points[i].x, points[i].y);
		Sleep(1);
	}

	return 0;
}