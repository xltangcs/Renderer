#pragma once

#include <string>
#include <vector>
#include <memory>

#include "imgui.h"
#include <glad/glad.h>
#include <GLFW//glfw3.h>
#include <glm/glm.hpp>

#include "ImGuiLayer.h"

class Application {
public:
	Application(const std::string& applicationname = "App");
	~Application();

	static Application& Get();

	void Run();
	void Close();
	void PushImGuiLayer(const std::shared_ptr<ImGuiLayer>& layer);

	GLFWwindow* GetGLFWwindow() { return m_GLFWwindow; }

	

private:
	void Init();

private:
	float m_TimeStep = 0.0f;
	float m_FrameTime = 0.0f;
	float m_LastFrameTime = 0.0f;

	std::string m_ApplicationName;
	GLFWwindow* m_GLFWwindow = nullptr;
	bool m_Running = false;
	std::vector<std::shared_ptr<ImGuiLayer>> m_ImGuiLayerVector;
};