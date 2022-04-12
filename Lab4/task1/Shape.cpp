#include "includes.h"
#include "Shape.h"

Shape::Shape(float size)
	: m_size(size)
{
	constexpr glm::vec4 defaultColor{ 1, 1, 1, 1 };
}

void Shape::Draw() const
{

	/*
	   Y
	   |
	   |
	   |
	   +---X
	  /
	 /
	Z
	   3----2
	  /    /|
	 /    / |
	7----6  |
	|  0 |  1
	|    | /
	|    |/
	4----5
	*/
	// Массив координат вершин
	float sizeSqrt = float(sqrt(2));
	float maxCoordValue = 1 + 2 * sizeSqrt;
	float middleCoordValue = 1 + sizeSqrt;
	float minCoordValue = 1.0f;
	static float vertices[48][3] = {
		// back octagon
		{ -minCoordValue, -middleCoordValue, -maxCoordValue }, // 0
		{ +minCoordValue, -middleCoordValue, -maxCoordValue }, // 1
		{ +middleCoordValue, -minCoordValue, -maxCoordValue }, // 2
		{ +middleCoordValue, +minCoordValue, -maxCoordValue }, // 3
		{ +minCoordValue, +middleCoordValue, -maxCoordValue }, // 4
		{ -minCoordValue, +middleCoordValue, -maxCoordValue }, // 5
		{ -middleCoordValue, +minCoordValue, -maxCoordValue }, // 6
		{ -middleCoordValue, -minCoordValue, -maxCoordValue }, // 7
		// left octagon
		{ -maxCoordValue, -minCoordValue, -middleCoordValue }, // 8
		{ -maxCoordValue, +minCoordValue, -middleCoordValue }, // 9
		{ -maxCoordValue, +middleCoordValue, -minCoordValue }, // 10
		{ -maxCoordValue, +middleCoordValue, +minCoordValue }, // 11
		{ -maxCoordValue, +minCoordValue, +middleCoordValue }, // 12
		{ -maxCoordValue, -minCoordValue, +middleCoordValue }, // 13
		{ -maxCoordValue, -middleCoordValue, +minCoordValue }, // 14
		{ -maxCoordValue, -middleCoordValue, -minCoordValue }, // 15
		// bottom octagon
		{ -minCoordValue, -maxCoordValue, -middleCoordValue }, // 16
		{ +minCoordValue, -maxCoordValue, -middleCoordValue }, // 17
		{ +middleCoordValue, -maxCoordValue, -minCoordValue }, // 18
		{ +middleCoordValue, -maxCoordValue, +minCoordValue }, // 19
		{ +minCoordValue, -maxCoordValue, +middleCoordValue }, // 20
		{ -minCoordValue, -maxCoordValue, +middleCoordValue }, // 21
		{ -middleCoordValue, -maxCoordValue, +minCoordValue }, // 22
		{ -middleCoordValue, -maxCoordValue, -minCoordValue }, // 23
		// right octagon
		{ +maxCoordValue, -minCoordValue, -middleCoordValue }, // 24
		{ +maxCoordValue, +minCoordValue, -middleCoordValue }, // 25
		{ +maxCoordValue, +middleCoordValue, -minCoordValue }, // 26
		{ +maxCoordValue, +middleCoordValue, +minCoordValue }, // 27
		{ +maxCoordValue, +minCoordValue, +middleCoordValue }, // 28
		{ +maxCoordValue, -minCoordValue, +middleCoordValue }, // 29
		{ +maxCoordValue, -middleCoordValue, +minCoordValue }, // 30
		{ +maxCoordValue, -middleCoordValue, -minCoordValue }, // 31
		// top octagon
		{ -minCoordValue, +maxCoordValue, -middleCoordValue }, // 32
		{ +minCoordValue, +maxCoordValue, -middleCoordValue }, // 33
		{ +middleCoordValue, +maxCoordValue, -minCoordValue }, // 34
		{ +middleCoordValue, +maxCoordValue, +minCoordValue }, // 35
		{ +minCoordValue, +maxCoordValue, +middleCoordValue }, // 36
		{ -minCoordValue, +maxCoordValue, +middleCoordValue }, // 37
		{ -middleCoordValue, +maxCoordValue, +minCoordValue }, // 38
		{ -middleCoordValue, +maxCoordValue, -minCoordValue }, // 39
		//front octagon
		{ -minCoordValue, -middleCoordValue, +maxCoordValue }, // 40
		{ +minCoordValue, -middleCoordValue, +maxCoordValue }, // 41
		{ +middleCoordValue, -minCoordValue, +maxCoordValue }, // 42
		{ +middleCoordValue, +minCoordValue, +maxCoordValue }, // 43
		{ +minCoordValue, +middleCoordValue, +maxCoordValue }, // 44
		{ -minCoordValue, +middleCoordValue, +maxCoordValue }, // 45
		{ -middleCoordValue, +minCoordValue, +maxCoordValue }, // 46
		{ -middleCoordValue, -minCoordValue, +maxCoordValue }, // 47
	};

	// Массив координат граней (в порядке, совпадающем с
	// порядком объявления их в массиве цветов)
	// индексы вершин граней перечисляются в порядке их обхода
	// против часовой стрелки (если смотреть на грань снаружи)
	static std::vector<std::vector<char>> faces = {
		{ 40, 41, 42, 43, 44, 45, 46, 47 }, // front octagon
		{ 0, 7, 6, 5, 4, 3, 2, 1 },			// back octagon
		{ 15, 14, 13, 12, 11, 10, 9, 8 },   // left octagon
		{ 24, 25, 26, 27, 28, 29, 30, 31 }, // right octagon
		{ 16, 17, 18, 19, 20, 21, 22, 23 },	// bottom octagon
		{ 39, 38, 37, 36, 35, 34, 33, 32 }, // top octagon
		{ 41, 40, 21, 20 },					// front-bottom square
		{ 43, 42, 29, 28 },					// front-right square
		{ 47, 46, 12, 13 },					// front-left square
		{ 45, 44, 36, 37 },					// front-top square
		{ 0, 1, 17, 16 },				    // back-bottom square
		{ 4, 5, 32, 33 },					// back-top square
		{ 6, 7, 8, 9 },					    // back-left square
		{ 2, 3, 25, 24 },					// back-right square
		{ 38, 39, 10, 11 },					// top-left square
		{ 34, 35, 27, 26 },					// top-right square
		{ 23, 22, 14, 15 },					// bottom-left square
		{ 19, 18, 31, 30 },					// bottom-right square
		{ 42, 41, 20, 19, 30, 29 },			// front-bottom-right hexagon
		{ 40, 47, 13, 14, 22, 21 },			// front-bottom-left hexagon
		{ 46, 45, 37, 38, 11, 12 },			// front-top-left hexagon
		{ 44, 43, 28, 27, 35, 36 },			// front-top-right hexagon
		{ 1, 2, 24, 31, 18, 17 },			// back-bottom-right hexagon
		{ 3, 4, 33, 34, 26, 25 },			// back-bottom-left hexagon
		{ 5, 6, 9, 10, 39, 32 },			// back-top-left hexagon
		{ 7, 0, 16, 23, 15, 8 },			// back-top-right hexagon
	};
	glEnable(GL_COLOR_MATERIAL);
	// Цвет вершины будет управлять диффузным и фоновым цветом материала
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(m_specularColor));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);

	int faceIndex = 0;
	for (auto face : faces)
	{
		glBegin(GL_POLYGON);
		glColor4f(1.0f, 0.0f, 0.0f, 0.1f);
		auto p0 = glm::make_vec3(vertices[face[0]]);
		auto p1 = glm::make_vec3(vertices[face[1]]);
		auto p2 = glm::make_vec3(vertices[face[2]]);
		p0 *= m_size * 0.5f;
		p1 *= m_size * 0.5f;
		p2 *= m_size * 0.5f;
		auto v01 = p1 - p0;
		auto v02 = p2 - p0;
		auto normal = glm::normalize(glm::cross(v01, v02));
		glNormal3fv(glm::value_ptr(normal));

		for (auto facePoint : face)
		{
			auto p = glm::make_vec3(vertices[facePoint]);
			p *= m_size * 0.5f;

			glVertex3fv(glm::value_ptr(p));
		}
		faceIndex++;
		glEnd();
	}

	faceIndex = 0;
	for (auto const& face : faces)
	{
		glBegin(GL_LINE_LOOP);
		glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
		for (auto const facePoint : face)
		{
			auto p = glm::make_vec3(vertices[facePoint]);
			p *= m_size * 0.5f;

			glVertex3fv(glm::value_ptr(p));
		}
		faceIndex++;
		glEnd();
	}
}

void Shape::SetSpecularColor(glm::vec4 color)
{
	m_specularColor = color;
}

void Shape::SetShininess(float shininess)
{
	m_shininess = shininess;
}
