#include "CoroutineSystem.h"

leap::CoroutineSystem* pCoroutineSystem{};

void StartCoroutine(leap::Coroutine<>&& coroutine)
{
	// push into gameobject vector of coroutines
	leap::IEnumerator ienum = coroutine.Value();
	// Register to coroutine.
	pCoroutineSystem->Register(&coroutine, std::move(ienum));
}

leap::CoroutineSystem::CoroutineSystem()
{
}

leap::CoroutineSystem::~CoroutineSystem()
{
}

void leap::CoroutineSystem::Register(leap::Coroutine<>* pCoroutine, leap::IEnumerator&& iEnumerator)
{
	m_Coroutines.emplace_back(CoPair{ pCoroutine,std::make_unique<IEnumerator>(iEnumerator) });
}

void leap::CoroutineSystem::Unregister(leap::Coroutine<>* pCoroutine)
{
	auto it = std::find_if(m_Coroutines.begin(), m_Coroutines.end(), [&](const CoPair& copair) { return copair.coroutine == pCoroutine; });
	m_Coroutines.erase(it);
}
