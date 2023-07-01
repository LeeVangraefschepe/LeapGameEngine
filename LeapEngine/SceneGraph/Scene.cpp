#include "Scene.h"

leap::Scene::Scene(std::string name) : m_Name(std::move(name))
{
	m_RootObject = std::make_unique<GameObject>(m_Name);
}

leap::GameObject* leap::Scene::CreateGameObject(const std::string& name) const
{
	return m_RootObject->CreateChild(name);
}

void leap::Scene::RemoveAll()
{
	m_RootObject.reset();
	m_RootObject = std::make_unique<GameObject>(m_Name);
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
