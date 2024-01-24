#include "ImGuiSceneChooser.h"

#include <ImGui/imgui.h>

#include <SceneGraph/SceneManager.h>

void unag::ImGuiSceneChooser::OnGUI()
{
	ImGui::Begin("Choose scene");

	if (ImGui::Button("Extended Camera Logic Test"))
	{
		leap::SceneManager::GetInstance().LoadScene(1);
	}

	ImGui::End();
}
