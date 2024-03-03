#include <Windows.h>
#include <cstdio>

#include <memory>
#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Camera.h"
#include "Core/Application.h"

#include "App/Renderer.h"
#include "App/Scene/BaseScene.h"

class MyImGuiLayer : public ImGuiLayer
{
public:
	MyImGuiLayer()
		:m_Camera(45.0f, 0.1f, 100.0f)
	{
		//m_Scene = CreatBaseScene();
	}

	virtual void OnUpdate(float ts) override
	{
		int width, height;
		auto window = Application::Get().GetGLFWwindow();
		glfwGetFramebufferSize(window, &width, &height);
		m_Width = width; m_Height = height;
		glViewport(0, 0, m_Width, m_Height);

		m_Camera.OnUpdate(ts);
		m_Renderer.OnResize(m_Width, m_Height);
	}

	virtual void ShowUI(float ts) override
	{
		ImGui::Begin("Setting");

		ImGui::Text("The average fps: %.3f", ImGui::GetIO().Framerate);
		ImGui::Separator();
		ImGui::DragFloat3("Camera Position", glm::value_ptr(m_Camera.GetPosition()));
		ImGui::DragFloat3("Camera Direction", glm::value_ptr(m_Camera.GetDirection()));
		ImGui::Checkbox("Camera Rotation", &m_Camera.GetIsRotation());

		ImGui::End();

		ImGui::Begin("Viewport");
		auto image = m_Renderer.GetImage();

		ImGui::Image((ImTextureID)image->GetTextureID(), { (float)image->GetWidth(), (float)image->GetHeight() });

		ImGui::End();

	}

	virtual void Render(float ts) override
	{
		m_Renderer.Render(m_Camera, m_Scene);
	}

private:
	unsigned int m_Width = 10, m_Height = 10;

	Camera m_Camera;
	Scene m_Scene;
	Renderer m_Renderer;
};

int main()
{
	static Application* App = new Application("Renderer");
	std::shared_ptr<MyImGuiLayer> myimguilayer = std::make_shared<MyImGuiLayer>();

	App->PushImGuiLayer(myimguilayer);
	App->Run();

	return 0;
}