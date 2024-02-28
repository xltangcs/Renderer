#pragma once

#include <glm/glm.hpp>

#include "Core/Application.h"

class Camera
{
public:
	bool isCameraMoved = false;
public:
	Camera(float verticalFOV = 45.0f, float nearClip = 0.1f, float farClip = 100.0f, bool isrotation = false);

	void OnUpdate(float ts);
	void OnResize(int width, int height);
	void ProcessMouseScroll();

	const glm::mat4& GetProjection() const { return m_Projection; }
	const glm::mat4& GetInverseProjection() const { return m_InverseProjection; }
	const glm::mat4& GetView() const { return m_View; }
	const glm::mat4& GetInverseView() const { return m_InverseView; }

	glm::vec3& GetPosition() { return m_Position; }
	glm::vec3& GetDirection() { return m_ForwardDirection; }
	bool& GetIsRotation() { return isRotation; }
	const float& GetFov() const { return m_VerticalFOV; }
	int& GetWidth() { return m_ViewportWidth; }
	int& GetHeight() { return m_ViewportHeight; }

	float GetRotationSpeed();

private:
	void RecalculateProjection();
	void RecalculateView();
private:
	glm::mat4 m_Projection{ 1.0f };
	glm::mat4 m_View{ 1.0f };
	glm::mat4 m_InverseProjection{ 1.0f };
	glm::mat4 m_InverseView{ 1.0f };

	float m_VerticalFOV = 45.0f;
	float m_NearClip = 0.1f;
	float m_FarClip = 100.0f;
	bool isRotation;

	glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_ForwardDirection{ 0.0f, 0.0f, 0.0f };

	glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };

	int m_ViewportWidth = 0, m_ViewportHeight = 0;
};