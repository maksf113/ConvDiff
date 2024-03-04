#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Domain.h"

class Renderer
{
private:
	GLFWwindow* m_window;
	GLuint m_width;
	GLuint m_height;
	ImGuiIO m_IO;
	std::vector<std::vector<float>> m_data;
	float m_domainWidth;
	int m_time = 0;
	Domain m_domain;
public:
	Renderer(GLuint width, GLuint height, const Domain& D);
	void LinePlot();
	void Render();
	~Renderer();
};

Renderer::Renderer(GLuint width, GLuint height, const Domain& D) : m_width(width), m_height(height), m_domain(D)
{
	m_data.resize(D.TimeSteps());
	for (auto& xGrid : m_data)
		xGrid.resize(D.Width());
	for (int i = 0; i < D.TimeSteps(); i++)
	{
		for (int j = 0; j < D.Width(); j++)
		{
			m_data[i][j] = float(D(i, j));
		}
	}
		
			

	m_domainWidth = float(D.DomainWidth());

	if (!glfwInit()) { exit(EXIT_FAILURE); }
	// Tell GLFW what version of OpenGL we are using 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a GLFWwindow object
	m_window = glfwCreateWindow(width, height, "Convection - Diffusion", NULL, NULL);
	// Introduce the window into the current context
	glfwMakeContextCurrent(m_window);
	// Specify the viewport of OpenGL in the Window

	glViewport(0, 0, width, height);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	// ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	m_IO = ImGui::GetIO(); (void)m_IO;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
}

void Renderer::Render()
{
	double crntTime = 0.0;
	double prevTime = 0.0;
	double dt = 0.0;
	while (!glfwWindowShouldClose(m_window))
	{
		crntTime = glfwGetTime();
		dt = crntTime - prevTime;

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("My Window", nullptr);
		ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)); // Set position (x, y)
		ImGui::SetWindowSize(ImVec2(float(m_width), float(m_height)));
		ImGui::Text("Hello, world!");
		ImGui::SliderInt("Time step", &m_time, 0, m_data.size() - 1);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_IO.Framerate, m_IO.Framerate);
		LinePlot();
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
}

void Renderer::LinePlot()
{
	std::vector<float> x(m_data[0].size());
	float dx = m_domainWidth / x.size();
	for (int i = 0; i < x.size(); i++)
		x[i] = i * dx;

	if (ImPlot::BeginPlot("Plot"))
	{
		ImPlot::SetupAxes("x", "u");
		ImPlot::PlotLine("u(x)", x.data(), m_data[m_time].data(), x.size());
		ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
		ImPlot::EndPlot();
	}
}

Renderer::~Renderer()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	ImPlot::DestroyContext();
	glfwDestroyWindow(m_window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}