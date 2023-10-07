#include "InfoUI.h"
#include "ImGui/imgui.h"

void unag::InfoUI::OnGUI()
{
	ImGui::SetNextWindowSize(ImVec2(250, 100));
	ImGui::Begin("Welcome to unag!");
	ImGui::Text("For if you didn't know unag stands\nfor unnamed adventure game.");
	ImGui::End();
}