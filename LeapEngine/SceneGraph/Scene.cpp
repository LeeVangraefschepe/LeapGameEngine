#include "Scene.h"

#include <Interfaces/IPhysics.h>
#include "../ServiceLocator/ServiceLocator.h"

leap::Scene::Scene(const char* name)
{
	m_pRootObject = std::make_unique<GameObject>(name);
	ServiceLocator::GetPhysics().CreateScene();
}

leap::Scene::~Scene()
{
	m_pRootObject->OnDestroy();
}

leap::GameObject* leap::Scene::CreateGameObject(const char* name) const
{
	return m_pRootObject->CreateChild(name);
}

void leap::Scene::RemoveAll()
{
	m_pRootObject->OnDestroy();
	const char* name = m_pRootObject->GetRawName();
	m_pRootObject.reset();
	m_pRootObject = std::make_unique<GameObject>(name);
}

leap::GameObject* leap::Scene::GetRootObject() const
{
	return m_pRootObject.get();
}

void leap::Scene::OnFrameStart() const
{
	m_pRootObject->OnFrameStart();
}

void leap::Scene::FixedUpdate() const
{
	m_pRootObject->FixedUpdate();
}

void leap::Scene::Update() const
{
	m_pRootObject->Update();
}

void leap::Scene::LateUpdate() const
{
	m_pRootObject->LateUpdate();
}

void leap::Scene::OnGUI() const
{
	m_pRootObject->OnGUI();
}

void leap::Scene::OnFrameEnd() const
{
	m_pRootObject->OnFrameEnd();
}
