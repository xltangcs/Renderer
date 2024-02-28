#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//#include <iostream>

static float m_Yoffset = 0.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_Yoffset = static_cast<float>(yoffset);
	//std::cout << Yoffset << std::endl;
	//printf("Mouse scroll\n");
}

Camera::Camera(float verticalFOV, float nearClip, float farClip, bool isrotation)
	: m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip), isRotation(isrotation)
{
	m_ForwardDirection = glm::vec3(0, 0, -1);
	m_Position = glm::vec3(0, 0, 3);

	auto window = Application::Get().GetGLFWwindow();
	glfwSetScrollCallback(window, scroll_callback);

	RecalculateView();
}

void Camera::OnUpdate(float ts)
{
	int width, height;
	auto window = Application::Get().GetGLFWwindow();
	glfwGetFramebufferSize(window, &width, &height);
	OnResize(width, height);

	this->isCameraMoved = false;

	auto IsKeyDown = [&window](int key) ->bool {
		return glfwGetKey(window, key) == GLFW_PRESS || glfwGetKey(window, key) == GLFW_REPEAT;
	};

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	glm::vec2 mousePos((float)x, (float)y);

	glm::vec2 delta = (mousePos - m_LastMousePosition) * 0.002f;
	m_LastMousePosition = mousePos;

	static bool isFirst = true;
	// mouse right PRESS status into editing mode
	if (!isFirst && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}
	isFirst = false;

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);

	constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
	glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

	float speed = 5.0f;

	// Movement
	if (IsKeyDown(GLFW_KEY_W))
	{
		m_Position += m_ForwardDirection * speed * ts;
		isCameraMoved = true;
	}
	else if (IsKeyDown(GLFW_KEY_S))
	{
		m_Position -= m_ForwardDirection * speed * ts;
		isCameraMoved = true;
	}
	if (IsKeyDown(GLFW_KEY_A))
	{
		m_Position -= rightDirection * speed * ts;
		isCameraMoved = true;
	}
	else if (IsKeyDown(GLFW_KEY_D))
	{
		m_Position += rightDirection * speed * ts;
		isCameraMoved = true;
	}

	if (IsKeyDown(GLFW_KEY_Q))
	{
		m_Position -= upDirection * speed * ts;
		isCameraMoved = true;
	}
	else if (IsKeyDown(GLFW_KEY_E))
	{
		m_Position += upDirection * speed * ts;
		isCameraMoved = true;
	}

	if (isRotation)
	{
		// Rotation
		if (delta.x != 0.0f || delta.y != 0.0f)
		{
			float pitchDelta = delta.y * GetRotationSpeed();
			float yawDelta = delta.x * GetRotationSpeed();

			glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
				glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
			m_ForwardDirection = glm::rotate(q, m_ForwardDirection);

			isCameraMoved = true;
		}
	}
	if (m_Yoffset)
	{
		isCameraMoved = true;
		ProcessMouseScroll();
	}
	

	if (isCameraMoved)
	{
		RecalculateView();
	}
}

void Camera::OnResize(int width, int height)
{
	if (width == m_ViewportWidth && height == m_ViewportHeight)
		return;

	m_ViewportWidth = width;
	m_ViewportHeight = height;

	RecalculateProjection();
}

void Camera::ProcessMouseScroll()
{
	m_VerticalFOV -= (float)m_Yoffset * 0.25f;
	if (m_VerticalFOV < 1.0f)
		m_VerticalFOV = 1.0f;
	if (m_VerticalFOV > 80.0f)
		m_VerticalFOV = 80.0f;

	RecalculateProjection();
}

float Camera::GetRotationSpeed()
{
	return 0.3f;
}

void Camera::RecalculateProjection()
{
	m_Projection = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth, (float)m_ViewportHeight, m_NearClip, m_FarClip);
	m_InverseProjection = glm::inverse(m_Projection);
}

void Camera::RecalculateView()
{
	m_View = glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0, 1, 0));
	m_InverseView = glm::inverse(m_View);
}
