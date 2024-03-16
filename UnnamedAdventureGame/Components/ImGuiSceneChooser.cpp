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
	else if (ImGui::Button("Particle Test"))
	{
		leap::SceneManager::GetInstance().LoadScene(2);
	}
	else if (ImGui::Button("Mesh Loading Test"))
	{
		leap::SceneManager::GetInstance().LoadScene(3);
	}

	ImGui::End();
}
