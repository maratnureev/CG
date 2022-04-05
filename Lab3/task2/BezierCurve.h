#pragma once
#include <vector>

struct Point {
	float x;
	float y;
};

class BezierCurve
{
public:
	void RegisterPoint(float x, float y);
	void ClearPoints();
	std::vector<Point> GetCurve();
private:
	std::vector<Point> points;
};

