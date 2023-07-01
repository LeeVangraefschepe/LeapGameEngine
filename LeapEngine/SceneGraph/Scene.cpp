#include "Scene.h"

leap::Scene::Scene(const std::string& name)
{
	m_RootObject = std::make_unique<GameObject>(name);
}

leap::GameObject* leap::Scene::CreateGameObject(const std::string& name) const
{
	return m_RootObject->CreateChild(name);
}

void leap::Scene::RemoveAll()
{
	std::string name = m_RootObject->GetName();
	m_RootObject.reset();
	m_RootObject = std::make_unique<GameObject>(name);
}

const leap::GameObject* leap::Scene::GetRootObject() const
{
	return m_RootObject.get();
}

void leap::Scene::OnFrameStart() const
{
	m_RootObject->OnFrameStart();
}

void leap::Scene::FixedUpdate() const
{
	m_RootObject->FixedUpdate();
}

void leap::Scene::Update() const
{
	m_RootObject->Update();
}

void leap::Scene::LateUpdate() const
{
	m_RootObject->LateUpdate();
}

void leap::Scene::Render() const
{
	m_RootObject->Render();
}

void leap::Scene::OnGUI() const
{
	m_RootObject->OnGUI();
}

void leap::Scene::OnFrameEnd() const
{
	m_RootObject->OnFrameEnd();
}
