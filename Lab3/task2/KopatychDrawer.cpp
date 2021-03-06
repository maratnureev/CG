#define _USE_MATH_DEFINES
#include "KopatychDrawer.h"
#include "BezierCurve.h"
#include <vector>
#include <cmath>
#include <GLFW/glfw3.h>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <algorithm>

struct Color {
	Color(int r, int g, int b)
		: r(float(r) / 255)
		, g(float(g) / 255)
		, b(float(b) / 255)
	{
	};
	Color(float r, float g, float b)
		: r(r), g(g), b(b)
	{
	};

	float r;
	float g;
	float b;
};

void DrawLine(std::vector<Point> points, const Color color, bool isFilled = false, float lineWidth = 10.0f)
{
	glLineWidth(lineWidth);
	glColor3f(color.r, color.g, color.b);
	if (isFilled)
	{
		glBegin(GL_TRIANGLE_FAN);
		float x = 0, y = 0;
		size_t pointsSize = points.size();
		for (auto& point : points)
		{
			x += point.x;
			y += point.y;
		}
		glVertex2d(x / (float)pointsSize - 1, y / (float)pointsSize - 1);
		glVertex2d(points[pointsSize - 1].x, points[pointsSize - 1].y);
	}
	else
		glBegin(GL_LINE_STRIP);
	for (auto& point : points)
	{
		glVertex2d(point.x, point.y);
	}

	glEnd();
}

void DrawEllipse(float xCenter, float yCenter, float rx, float ry, const Color color, bool isFilled = true)
{
	glLineWidth(8.0f);
	int points = 360;
	const float step = static_cast<float>(2 * M_PI / points);
	glColor3f(color.r, color.g, color.b);
	if (isFilled)
	{
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(xCenter, yCenter);
	}
	else
		glBegin(GL_LINE_STRIP);

	for (float angle = 0; angle <= 2 * M_PI; angle += step)
	{
		float a = (fabsf(static_cast<float>(angle - 2 * M_PI)) < 1e-5) ? 0 : angle;
		const float dx = rx * cosf(a);
		const float dy = ry * sinf(a);
		glVertex2f(dx + xCenter, dy + yCenter);
	}
	glEnd();
}

void DrawTriangle(Point p1, Point p2, Point p3, const Color color)
{
	glLineWidth(6.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glVertex2f(p3.x, p3.y);

	glEnd();
}

void KopatychDrawer::DrawKopatych()
{
	DrawLegs();
	DrawHat();
	//body outcircle
	DrawEllipse(500, 500, 334, 334, Color{ 148, 59, 43 }, false);
	DrawEars();
	//body
	DrawEllipse(500, 500, 330, 330, Color{ 227, 105, 19 }, true);
	DrawEyes();
	DrawEyeBrows();
	//nose
	DrawEllipse(500, 471, 100, 60, Color{ 95, 56, 44 }, true);
	DrawCheeks();
	DrawSmile();
	DrawArms();
}

void KopatychDrawer::DrawLegs()
{
	BezierCurve leftLegCurve1;
	leftLegCurve1.RegisterPoint(462, 831);
	leftLegCurve1.RegisterPoint(480, 930);
	leftLegCurve1.RegisterPoint(480, 960);
	leftLegCurve1.RegisterPoint(445, 970);
	std::vector<Point> leftLegLine = {
		Point{445, 970},
		Point{335, 970},
	};
	BezierCurve leftLegCurve2;
	leftLegCurve2.RegisterPoint(335, 970);
	leftLegCurve2.RegisterPoint(300, 970);
	leftLegCurve2.RegisterPoint(300, 900);
	leftLegCurve2.RegisterPoint(300, 885);
	leftLegCurve2.RegisterPoint(400, 875);
	BezierCurve leftLegCurve3;
	leftLegCurve3.RegisterPoint(370, 878);
	leftLegCurve3.RegisterPoint(380, 840);
	leftLegCurve3.RegisterPoint(393, 817);
	auto leftLegCurve = leftLegCurve1.GetCurve();
	auto l2 = leftLegCurve2.GetCurve();
	auto l3 = leftLegCurve3.GetCurve();
	leftLegCurve.insert(leftLegCurve.end(), leftLegLine.begin(), leftLegLine.end());
	leftLegCurve.insert(leftLegCurve.end(), l2.begin(), l2.end());
	leftLegCurve.insert(leftLegCurve.end(), l3.begin(), l3.end());
	DrawLine(leftLegCurve, Color{ 227, 105, 19 }, true);
	DrawLine(leftLegCurve, Color{ 148, 59, 43 }, false, 5.0f);

	BezierCurve rightLegCurve1;
	rightLegCurve1.RegisterPoint(538, 831);
	rightLegCurve1.RegisterPoint(520, 930);
	rightLegCurve1.RegisterPoint(520, 960);
	rightLegCurve1.RegisterPoint(555, 970);
	std::vector<Point> rightLegLine = {
		Point{555, 970},
		Point{665, 970},
	};
	BezierCurve rightLegCurve2;
	rightLegCurve2.RegisterPoint(665, 970);
	rightLegCurve2.RegisterPoint(700, 970);
	rightLegCurve2.RegisterPoint(700, 900);
	rightLegCurve2.RegisterPoint(700, 885);
	rightLegCurve2.RegisterPoint(600, 875);
	BezierCurve rightLegCurve3;
	rightLegCurve3.RegisterPoint(630, 878);
	rightLegCurve3.RegisterPoint(620, 840);
	rightLegCurve3.RegisterPoint(607, 817);
	auto rightLegCurve = rightLegCurve1.GetCurve();
	auto r2 = rightLegCurve2.GetCurve();
	auto r3 = rightLegCurve3.GetCurve();
	rightLegCurve.insert(rightLegCurve.end(), rightLegLine.begin(), rightLegLine.end());
	rightLegCurve.insert(rightLegCurve.end(), r2.begin(), r2.end());
	rightLegCurve.insert(rightLegCurve.end(), r3.begin(), r3.end());
	DrawLine(rightLegCurve, Color{ 227, 105, 19 }, true);
	DrawLine(rightLegCurve, Color{ 148, 59, 43 }, false, 5.0f);
}

void KopatychDrawer::DrawHat()
{
	DrawEllipse(500, 90, 160, 40, Color{ 240, 156, 18 }, true);
	DrawEllipse(500, 90, 160, 40, Color{ 205, 112, 28 }, false);
	std::vector<Point> hatPolygon = {
		Point{340, 90},
		Point{335, 150},
		Point{665, 150},
		Point{660, 90}
	};
	DrawLine(hatPolygon, Color{ 240, 156, 18 }, true);
	DrawLine(hatPolygon, Color{ 205, 112, 28 }, false);

	DrawEllipse(500, 210, 270, 100, Color{ 240, 156, 18 }, true);
	DrawEllipse(500, 210, 270, 100, Color{ 205, 112, 28 }, false);
}

void KopatychDrawer::DrawEars()
{
	DrawEllipse(330, 190, 35, 35, Color{ 227, 105, 19 }, true);
	DrawEllipse(670, 190, 35, 35, Color{ 227, 105, 19 }, true);
	DrawEllipse(330, 190, 35, 35, Color{ 148, 59, 43 }, false);
	DrawEllipse(670, 190, 35, 35, Color{ 148, 59, 43 }, false);
}

void KopatychDrawer::DrawEyes()
{
	DrawEllipse(420, 370, 90, 90, Color{ 148, 59, 43 }, false);
	DrawEllipse(580, 370, 90, 90, Color{ 148, 59, 43 }, false);
	DrawEllipse(420, 370, 90, 90, Color{ 255, 255, 255 }, true);
	DrawEllipse(580, 370, 90, 90, Color{ 255, 255, 255 }, true);
	std::vector<Point> eyeLinePoints = {
		Point{500, 321},
		Point{500, 437}
	};
	DrawLine(eyeLinePoints, Color{ 148, 59, 43 }, false, 5.0f);
	DrawEllipse(450, 360, 25, 40, Color{ 0,0,0 }, true);
	DrawEllipse(550, 360, 25, 40, Color{ 0,0,0 }, true);
	DrawEllipse(564, 339, 7, 10, Color{ 255,255,255 }, true);
	DrawEllipse(464, 339, 7, 10, Color{ 255,255,255 }, true);
}

void KopatychDrawer::DrawEyeBrows()
{
	std::vector<Point> leftEyeBrown = {
		Point{360, 237},
		Point{456, 212},
		Point{460, 255},
		Point{360, 275}
	};
	DrawLine(leftEyeBrown, Color{ 95, 56, 44 }, true);
	std::vector<Point> rightEyeBrown = {
		Point{640, 237},
		Point{544, 212},
		Point{540, 255},
		Point{640, 275}
	};
	DrawLine(rightEyeBrown, Color{ 95, 56, 44 }, true);
}

void KopatychDrawer::DrawCheeks()
{
	BezierCurve leftCheekCurve;
	leftCheekCurve.RegisterPoint(280, 380);
	leftCheekCurve.RegisterPoint(470, 380);
	leftCheekCurve.RegisterPoint(470, 550);
	leftCheekCurve.RegisterPoint(330, 570);
	DrawLine(leftCheekCurve.GetCurve(), Color{ 227, 105, 19 }, true);
	DrawLine(leftCheekCurve.GetCurve(), Color{ 148, 59, 43 }, false, 5.0f);
	BezierCurve rightCheekCurve;
	rightCheekCurve.RegisterPoint(720, 380);
	rightCheekCurve.RegisterPoint(530, 380);
	rightCheekCurve.RegisterPoint(530, 550);
	rightCheekCurve.RegisterPoint(670, 570);
	DrawLine(rightCheekCurve.GetCurve(), Color{ 227, 105, 19 }, true);
	DrawLine(rightCheekCurve.GetCurve(), Color{ 148, 59, 43 }, false, 5.0f);
}

void KopatychDrawer::DrawSmile()
{
	BezierCurve smileCurve1;
	smileCurve1.RegisterPoint(415, 520);
	smileCurve1.RegisterPoint(455, 600);
	smileCurve1.RegisterPoint(545, 600);
	smileCurve1.RegisterPoint(585, 520);
	BezierCurve smileCurve2;
	smileCurve2.RegisterPoint(415, 520);
	smileCurve2.RegisterPoint(455, 580);
	smileCurve2.RegisterPoint(545, 580);
	smileCurve2.RegisterPoint(585, 520);
	BezierCurve smileCurve3;
	smileCurve3.RegisterPoint(415, 520);
	smileCurve3.RegisterPoint(455, 587);
	smileCurve3.RegisterPoint(545, 587);
	smileCurve3.RegisterPoint(585, 520);
	BezierCurve smileCurve4;
	smileCurve4.RegisterPoint(415, 520);
	smileCurve4.RegisterPoint(455, 595);
	smileCurve4.RegisterPoint(545, 595);
	smileCurve4.RegisterPoint(585, 520);
	DrawLine(smileCurve1.GetCurve(), Color{ 95, 56, 44 }, false, 5.0f);
	DrawLine(smileCurve2.GetCurve(), Color{ 95, 56, 44 }, false, 5.0f);
	DrawLine(smileCurve3.GetCurve(), Color{ 95, 56, 44 }, false, 5.0f);
	DrawLine(smileCurve4.GetCurve(), Color{ 95, 56, 44 }, false, 5.0f);
}

void KopatychDrawer::DrawArms()
{
	BezierCurve armCurve1;
	armCurve1.RegisterPoint(165, 480);
	armCurve1.RegisterPoint(110, 440);
	armCurve1.RegisterPoint(50, 550);
	armCurve1.RegisterPoint(50, 650);
	BezierCurve armCurve2;
	armCurve2.RegisterPoint(50, 650);
	armCurve2.RegisterPoint(60, 735);
	armCurve2.RegisterPoint(110, 735);
	BezierCurve armCurve3;
	armCurve3.RegisterPoint(110, 735);
	armCurve3.RegisterPoint(165, 735);
	armCurve3.RegisterPoint(165, 675);
	BezierCurve armCurve4;
	armCurve4.RegisterPoint(165, 675);
	armCurve4.RegisterPoint(135, 600);
	armCurve4.RegisterPoint(165, 530);
	BezierCurve armCurve5;
	armCurve5.RegisterPoint(165, 530);
	armCurve5.RegisterPoint(175, 505);
	armCurve5.RegisterPoint(165, 480);
	auto armCurve = armCurve1.GetCurve();
	auto a2 = armCurve2.GetCurve();
	auto a3 = armCurve3.GetCurve();
	auto a4 = armCurve4.GetCurve();
	auto a5 = armCurve5.GetCurve();
	armCurve.insert(armCurve.end(), a2.begin(), a2.end());
	armCurve.insert(armCurve.end(), a3.begin(), a3.end());
	armCurve.insert(armCurve.end(), a4.begin(), a4.end());
	armCurve.insert(armCurve.end(), a5.begin(), a5.end());
	DrawLine(armCurve, Color{ 148, 59, 43 }, false);
	DrawLine(armCurve, Color{ 227, 105, 19 }, true);
	BezierCurve leftFingerCurve1;
	leftFingerCurve1.RegisterPoint(90, 733);
	leftFingerCurve1.RegisterPoint(65, 695);
	leftFingerCurve1.RegisterPoint(75, 670);
	DrawLine(leftFingerCurve1.GetCurve(), Color{ 148, 59, 43 }, false, 4.0f);
	BezierCurve leftFingerCurve2;
	leftFingerCurve2.RegisterPoint(140, 732);
	leftFingerCurve2.RegisterPoint(115, 700);
	leftFingerCurve2.RegisterPoint(120, 685);
	DrawLine(leftFingerCurve2.GetCurve(), Color{ 148, 59, 43 }, false, 4.0f);


	BezierCurve armCurve6;
	armCurve6.RegisterPoint(835, 480);
	armCurve6.RegisterPoint(890, 440);
	armCurve6.RegisterPoint(950, 550);
	armCurve6.RegisterPoint(950, 650);
	BezierCurve armCurve7;
	armCurve7.RegisterPoint(950, 650);
	armCurve7.RegisterPoint(940, 735);
	armCurve7.RegisterPoint(890, 735);
	BezierCurve armCurve8;
	armCurve8.RegisterPoint(890, 735);
	armCurve8.RegisterPoint(835, 735);
	armCurve8.RegisterPoint(835, 675);
	BezierCurve armCurve9;
	armCurve9.RegisterPoint(835, 675);
	armCurve9.RegisterPoint(865, 600);
	armCurve9.RegisterPoint(835, 530);
	BezierCurve armCurve10;
	armCurve10.RegisterPoint(835, 530);
	armCurve10.RegisterPoint(825, 505);
	armCurve10.RegisterPoint(835, 480);
	auto rightArmCurve = armCurve6.GetCurve();
	auto a6 = armCurve7.GetCurve();
	auto a7 = armCurve8.GetCurve();
	auto a8 = armCurve9.GetCurve();
	auto a9 = armCurve10.GetCurve();
	rightArmCurve.insert(rightArmCurve.end(), a6.begin(), a6.end());
	rightArmCurve.insert(rightArmCurve.end(), a7.begin(), a7.end());
	rightArmCurve.insert(rightArmCurve.end(), a8.begin(), a8.end());
	rightArmCurve.insert(rightArmCurve.end(), a9.begin(), a9.end());
	DrawLine(rightArmCurve, Color{ 148, 59, 43 }, false);
	DrawLine(rightArmCurve, Color{ 227, 105, 19 }, true);

	BezierCurve rightFingerCurve1;
	rightFingerCurve1.RegisterPoint(910, 733);
	rightFingerCurve1.RegisterPoint(935, 695);
	rightFingerCurve1.RegisterPoint(925, 670);
	DrawLine(rightFingerCurve1.GetCurve(), Color{ 148, 59, 43 }, false, 4.0f);
	BezierCurve rightFingerCurve2;
	rightFingerCurve2.RegisterPoint(860, 732);
	rightFingerCurve2.RegisterPoint(885, 700);
	rightFingerCurve2.RegisterPoint(880, 685);
	DrawLine(rightFingerCurve2.GetCurve(), Color{ 148, 59, 43 }, false, 4.0f);
}
