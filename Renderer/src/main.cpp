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
#include "Core/MyImGui.h"

#include "App/Renderer.h"
#include "App/Scene/BaseScene.h"
#include "App/Scene/PBRScene.h"

class MyImGuiLayer : public ImGuiLayer
{
public:
	MyImGuiLayer()
		:m_Camera(45.0f, 0.1f, 100.0f)
	{
		m_SceneNames.push_back("PBR Scene");
		createScenePtrs.push_back(PBRScene);

		m_SceneNames.push_back("Base Scene");
		createScenePtrs.push_back(BaseScene);

		m_Scene = BaseScene();
	}

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}

	virtual void ShowUI(float ts) override
	{
		GLFWwindow* window = Application::Get().GetGLFWwindow();

		const char* renderModeName[3] = { "Point", "Line", "Triangle" };
		const char* shaderModeName[4] = { "Lambert", "Phong", "Blin-Phong", "PBR"};

		ImGui::Begin("Setting");

		ImGui::Text("The average fps: %.3f", ImGui::GetIO().Framerate);
		ImGui::Separator();
		ImGui::DragFloat3("Camera Position", glm::value_ptr(m_Camera.GetPosition()));
		ImGui::DragFloat3("Camera Direction", glm::value_ptr(m_Camera.GetDirection()));
		ImGui::Checkbox("Camera Rotation", &m_Camera.GetIsRotation());
		ImGui::Separator();

		ImGui::Combo("Scene", &m_SceneIndex, m_SceneNames.data(), m_SceneNames.size());
		ImGui::Text("The number of triangles in the scene is : %d", m_Scene.m_Triangles.size());

		if (ImGui::Combo("Render Mode", (int*)&m_Renderer.rendermode, renderModeName, 3))
		{
			m_Renderer.isReset = true;
		}
		if (ImGui::Combo("Shader Mode", (int*)&m_Renderer.lightmodelmode, shaderModeName, 3))
		{
			m_Renderer.isReset = true;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			m_Renderer.isReset = true;
		}

		static glm::vec3 rotation(0.0f);
		static glm::vec3 translate (0.0f);
		static glm::vec3 scale(1.0f);
		//ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.01f, 0.01f);
		//ImGui::DragFloat3("Rotation", glm::value_ptr(rotation));
		//ImGui::DragFloat3("Translate", glm::value_ptr(translate), 0.01f);


		MyImGui::DrawVec3Control("Scale", scale, 1.0f);
		MyImGui::DrawVec3Control("Rotation", rotation);
		MyImGui::DrawVec3Control("Translate", translate);
		
		ImGui::DragFloat3("Light position", glm::value_ptr(m_Scene.m_Lights[0].position));
		ImGui::ColorEdit3("Light color", glm::value_ptr(m_Scene.m_Lights[0].color));

		m_Renderer.modelMatrix = glm::mat4(1.0f);
		m_Renderer.modelMatrix = glm::translate(m_Renderer.modelMatrix, translate) *
			glm::rotate(m_Renderer.modelMatrix, rotation.x, glm::vec3(1.0, 0.0, 0.0)) *
			glm::rotate(m_Renderer.modelMatrix, rotation.y, glm::vec3(0.0, 1.0, 0.0)) *
			glm::rotate(m_Renderer.modelMatrix, rotation.z, glm::vec3(0.0, 0.0, 1.0)) *
			glm::scale(m_Renderer.modelMatrix, scale);

		

		ImGui::End();

		ImGui::ShowDemoWindow();

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

		if (m_CurrentIndex != m_SceneIndex)
		{
			m_CurrentIndex = m_SceneIndex;
			m_Scene = createScenePtrs[m_CurrentIndex]();
			m_Renderer.isReset = true;
		}

		m_Renderer.Render(m_Camera, m_Scene);
		//m_Renderer.isReset = false;

	}

private:
	unsigned int m_Width = 10, m_Height = 10;

	std::vector<const char*> m_SceneNames;
	std::vector<Scene(*)()> createScenePtrs;
	int m_SceneIndex = 0;
	int m_CurrentIndex = -1;

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