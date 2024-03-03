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
		m_Scene = CreatBaseScene();
	}

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}

	virtual void ShowUI(float ts) override
	{
		const char* renderModeName[3] = { "Point", "Line", "Triangle" };
		ImGui::Begin("Setting");

		ImGui::Text("The average fps: %.3f", ImGui::GetIO().Framerate);
		ImGui::Separator();
		ImGui::DragFloat3("Camera Position", glm::value_ptr(m_Camera.GetPosition()));
		ImGui::DragFloat3("Camera Direction", glm::value_ptr(m_Camera.GetDirection()));
		ImGui::Checkbox("Camera Rotation", &m_Camera.GetIsRotation());
		ImGui::Separator();

		if (ImGui::Combo("Render Mode", (int*)&m_Renderer.rendermode, renderModeName, 3))
		{
			m_Renderer.isReset = true;
		}
		
		ImGui::End();

		ImGui::Begin("Viewport");

		m_Width = (uint32_t)ImGui::GetContentRegionAvail().x;
		m_Height = (uint32_t)ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetImage();
		if(image)
			ImGui::Image((ImTextureID)image->GetTextureID(), { (float)image->GetWidth(), (float)image->GetHeight() });

		ImGui::End();

	}

	virtual void Render(float ts) override
	{
		m_Renderer.OnResize(m_Width, m_Height);
		m_Camera.OnResize(m_Width, m_Height);

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