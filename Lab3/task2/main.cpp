#define _USE_MATH_DEFINES
#include <cmath>
#include <GLFW/glfw3.h>
#include <chrono>
#include <stdexcept>
#include "KopatychDrawer.h"

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
		SetupProjectionMatrix(width, height);
		glMatrixMode(GL_MODELVIEW);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0, 0, 1);
		DrawKopatychArmy();
	}
	
	//glCallList for kopatych
	static void DrawKopatychArmy()
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				KopatychDrawer kopatych;
				glPushMatrix();
				glTranslatef(i * 100, j * 100, 0);
				glScalef(0.1f, 0.1f, 0);
				kopatych.DrawKopatych();
				glPopMatrix();
			}
		}
	}

	static void SetupProjectionMatrix(int width, int height)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		const double aspectRatio = double(width) / double(height);
		double viewWidth = 1000.0;
		double viewHeight = viewWidth;
		if (aspectRatio > 1.0)
		{
			viewWidth = viewHeight * aspectRatio;
		}
		else
		{
			viewHeight = viewWidth / aspectRatio;
		}
		glOrtho(0, viewWidth, viewHeight, 0 , -1.0, 1.0);
	}
};



int main()
{
	GLFWInitializer initGLFW;
	Window window{ 1000, 1000, "Task2" };
	window.Run();
}
