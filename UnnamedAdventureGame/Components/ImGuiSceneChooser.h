#pragma once

#include <Components/Component.h>

namespace unag
{
	class ImGuiSceneChooser final : public leap::Component
	{
	public:
		ImGuiSceneChooser() = default;
		virtual ~ImGuiSceneChooser() = default;

		ImGuiSceneChooser(const ImGuiSceneChooser& other) = delete;
		ImGuiSceneChooser(ImGuiSceneChooser&& other) = delete;
		ImGuiSceneChooser& operator=(const ImGuiSceneChooser& other) = delete;
		ImGuiSceneChooser& operator=(ImGuiSceneChooser&& other) = delete;

	private:
		virtual void OnGUI() override;
	};
}