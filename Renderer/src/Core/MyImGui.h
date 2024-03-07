#pragma once
#include <string>

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/glm.hpp>

class MyImGui
{
public:
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
};