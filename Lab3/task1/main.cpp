#define _USE_MATH_DEFINES
#include <cmath>
#include <GLFW/glfw3.h>
#include <chrono>
#include <stdexcept>

struct Point {
	float x;
	float y;
};

struct Color {
	float r;
	float g;
	float b;
};

class GLFWInitializer final
{
public:
	GLFWInitializer()
	{
		if (!glfwInit())
		{
			throw std::runtime_error("Failed to initialize GLFW");
		}
	}

	GLFWInitializer(const GLFWInitializer&) = delete;
	GLFWInitializer& operator=(const GLFWInitializer&) = delete;

	~GLFWInitializer()
	{
		glfwTerminate();
	}
};

class BaseWindow
{
public:
	BaseWindow(int w, int h, const char* title)
		: m_window{ CreateWindow(w, h, title) }
	{
		if (!m_window)
		{
			throw std::runtime_error("Failed to create window");
		}
	}
	BaseWindow(const BaseWindow&) = delete;
	BaseWindow& operator=(const BaseWindow&) = delete;

	virtual ~BaseWindow()
	{
		glfwDestroyWindow(m_window);
	}

	void Run()
	{
		glfwMakeContextCurrent(m_window);
		OnRunStart();
		while (!glfwWindowShouldClose(m_window))
		{
			int w, h;
			glfwGetFramebufferSize(m_window, &w, &h);
			Draw(w, h);
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
		OnRunEnd();
	}

private:
	virtual void Draw(int width, int height) = 0;

	virtual void OnRunStart() {}
	virtual void OnRunEnd() {}

	static GLFWwindow* CreateWindow(int w, int h, const char* title)
	{
		return glfwCreateWindow(w, h, title, nullptr, nullptr);
	}
	GLFWwindow* m_window;
};

class Window : public BaseWindow
{
public:
	using BaseWindow::BaseWindow;

private:
	void OnRunStart() override
	{
		glClearColor(1, 1, 1, 1);
		glEnable(GL_LINE_SMOOTH);
	}

	void Draw(int width, int height) override
	{
		glViewport(0, 0, width, height);
		SetupProjectionMatrix(width, height);
		glMatrixMode(GL_MODELVIEW);

		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0, 0, 1);

		DrawCoordinateSystem();
		DrawHyperbola();
	}
	
	//class
	static void DrawHyperbola()
	{
		glBegin(GL_LINE_STRIP);
		for (float x = 0.1f; x < 10.0f; x += 0.1f)
		{
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2f(x, (1 / x));
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (float x = -0.1f; x > -10.0f; x -= 0.1f)
		{
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2f(x, (1 / x));
		}
		glEnd();
	}

	static void DrawCoordinateSystem()
	{
		DrawLine(Point{ -10.0f, 0.0f }, Point{ 10.0f, 0.0f }, Color{ 0.0f, 0.0f, 0.0f });
		DrawTriangle(Point{ 10.0f, 0.0f }, Point{9.75f, 0.25f }, Point{ 9.75f, -0.25f }, Color{ 0.0f, 0.0f, 0.0f });
		DrawLine(Point{ 0.0f, -10.0f }, Point{ 0.0f, 10.0f }, Color{ 0.0f, 0.0f, 0.0f });
		DrawTriangle(Point{ 0.0f, 10.0f }, Point{ 0.25f, 9.75f }, Point{ -0.25f, 9.75f }, Color{ 0.0f, 0.0f, 0.0f });

		for (int i = 0; i < 9; i++)
		{
			DrawLine(Point{ 1.0f + 1.0f * i, 0.25f }, Point{ 1.0f + 1.0f * i, -0.25f }, Color{ 0.0f, 0.0f, 0.0f });
			DrawLine(Point{ -1.0f + -1.0f * i, 0.25f }, Point{ -1.0f + -1.0f * i, -0.25f }, Color{ 0.0f, 0.0f, 0.0f });
			DrawLine(Point{ 0.25f, 1.0f + 1.0f * i }, Point{ -0.25f, 1.0f + 1.0f * i }, Color{ 0.0f, 0.0f, 0.0f });
			DrawLine(Point{ 0.25f, -1.0f + -1.0f * i }, Point{ -0.25f, -1.0f + -1.0f * i }, Color{ 0.0f, 0.0f, 0.0f });
		}

	}

	static void DrawLine(Point p1, Point p2, Color color)
	{
		glBegin(GL_LINE_STRIP);
		glColor3f(color.r, color.g, color.b);
		glVertex2f(p1.x, p1.y);

		glColor3f(color.r, color.g, color.b);
		glVertex2f(p2.x, p2.y);

		glEnd();
	}

	static void DrawTriangle(Point p1, Point p2, Point p3, Color color)
	{
		glBegin(GL_TRIANGLES);
		glColor3f(color.r, color.g, color.b);
		glVertex2f(p1.x, p1.y);

		glColor3f(color.r, color.g, color.b);
		glVertex2f(p2.x, p2.y);

		glColor3f(color.r, color.g, color.b);
		glVertex2f(p3.x, p3.y);

		glEnd();
	}

	static void SetupProjectionMatrix(int width, int height)
	{
		// Вычисляет матрицу ортографического преобразования такую, чтобы вписать квадратную область
		// [-1;+1] по обеим осям внутрь видового порта размером width*height пикселей
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		const double aspectRatio = double(width) / double(height);
		double viewWidth = 20.0;
		double viewHeight = viewWidth;
		if (aspectRatio > 1.0)
		{
			viewWidth = viewHeight * aspectRatio;
		}
		else
		{
			viewHeight = viewWidth / aspectRatio;
		}
		glOrtho(-viewWidth * 0.5, viewWidth * 0.5, -viewHeight * 0.5, viewHeight * 0.5, -1.0, 1.0);
	}
};

int main()
{
	GLFWInitializer initGLFW;
	Window window{ 800, 800, "Task1" };
	window.Run();
}
