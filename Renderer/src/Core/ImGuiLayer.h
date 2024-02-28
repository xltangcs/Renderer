#pragma once

#include <string>

class ImGuiLayer
{
public:
	ImGuiLayer() = default;
	virtual~ImGuiLayer() = default;

	virtual void OnUpdate(float ts) {};
	virtual void ShowUI(float ts) {};
	virtual void Render(float ts) {};
	
private:
};