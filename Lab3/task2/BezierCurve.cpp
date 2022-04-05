#include "BezierCurve.h"
#include <vector>
#include <iostream>

int Factorial(int x) {
	if (x > 1)
		return x * Factorial(x - 1);
	else
		return 1;
}

int BinomialCoefficiant(int n, int k)
{
	return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

void BezierCurve::RegisterPoint(float x, float y) {
	points.push_back(Point{ x,y });
}

void BezierCurve::ClearPoints() {
	points.clear();
}

std::vector<Point> BezierCurve::GetCurve() {
	std::vector<Point> curvePoints;

	float curveStart = points[0].x;

	float curveLength = 0.0f;

	for (int i = 0; i < points.size() - 1; i++)
	{
		curveLength += abs(sqrt(pow(points[i + 1].x - points[i].x, 2) + pow(points[i + 1].y - points[i].y, 2)));
	}
	float accuracy = 1.0f;

	float currentPoint = 0.0f;
	float currentCurveLength = 0.0f;

	while (currentPoint < curveLength)
	{
		float t = currentPoint / curveLength;

		int n = points.size() - 1;
		Point point = { 0.0f, 0.0f };

		for (int i = 0; i <= n; i++)
		{
			float coefficient = (BinomialCoefficiant(n, i) * pow(1 - t, n - i) * pow(t, i));
			point.x += coefficient * points[i].x;
			point.y += coefficient * points[i].y;
		}

		curvePoints.push_back(point);
		currentPoint += accuracy;
	}
	return curvePoints;
}