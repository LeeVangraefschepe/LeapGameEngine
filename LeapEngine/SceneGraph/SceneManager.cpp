#include "SceneManager.h"

#include <stdexcept>

#include "Scene.h"

void leap::SceneManager::AddScene(const std::string& name, const std::function<void(Scene&)>& load)
{
	m_Scenes.emplace_back(SceneData{ name, load });
}

void leap::SceneManager::LoadScene(unsigned index)
{
	if (index >= m_Scenes.size())
	{
		throw std::runtime_error("Tried to load scene that isn't known to the scenemanager");
		return;
	}

	const auto& sceneData = m_Scenes[index];
	m_Scene = std::make_unique<Scene>(sceneData.name);
	sceneData.load(*m_Scene);
}

void leap::SceneManager::OnFrameStart() const
{
	m_Scene->OnFrameStart();
}

void leap::SceneManager::FixedUpdate() const
{
	m_Scene->FixedUpdate();
}

void leap::SceneManager::Update() const
{
	m_Scene->Update();
}

void leap::SceneManager::LateUpdate() const
{
	m_Scene->LateUpdate();
}

void leap::SceneManager::Render() const
{
	m_Scene->Render();
}

void leap::SceneManager::OnGUI() const
{
	m_Scene->OnGUI();
}

void leap::SceneManager::OnFrameEnd() const
{
	m_Scene->OnFrameEnd();
}
