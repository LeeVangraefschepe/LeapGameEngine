#include "SceneManager.h"

#include <sstream>

#include "Debug.h"
#include "Scene.h"

leap::Scene* leap::SceneManager::GetActiveScene() const
{
	return m_Scene.get();
}

void leap::SceneManager::AddScene(const char* name, const std::function<void(Scene&)>& load)
{
	m_Scenes.emplace_back(SceneData{ name, load });
}

void leap::SceneManager::LoadScene(unsigned index)
{
	m_LoadScene = static_cast<int>(index);
}

void leap::SceneManager::LoadScene(const std::string& name)
{
	if (
		const auto it = std::ranges::find_if(m_Scenes, [&](const auto& scene)
			{
				return scene.name == name;
			}); it != m_Scenes.end()
		)
	{
		m_LoadScene = static_cast<int>(std::distance(m_Scenes.begin(), it));
		return;
	}

	std::stringstream ss{};
	ss << "LeapEngine Error: LoadScene failed to find a scene with name: " << name;
	Debug::LogError(ss.str());
}

void leap::SceneManager::OnFrameStart()
{
	if (m_LoadScene >= 0) { LoadInternalScene(); }
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

void leap::SceneManager::OnGUI() const
{
	m_Scene->OnGUI();
}

void leap::SceneManager::OnFrameEnd() const
{
	m_Scene->OnFrameEnd();
}

void leap::SceneManager::LoadInternalScene()
{
	if (static_cast<unsigned>(m_LoadScene) >= m_Scenes.size())
	{
		Debug::LogError("LeapEngine Error: SceneManager is loading a scene that doesn't exist");
		return;
	}

	const auto& sceneData = m_Scenes[m_LoadScene];
	m_LoadScene = -1;
	m_Scene = std::make_unique<Scene>(sceneData.name.c_str());
	sceneData.load(*m_Scene);
}

void leap::SceneManager::UnloadScene()
{
	m_Scene = nullptr;
}