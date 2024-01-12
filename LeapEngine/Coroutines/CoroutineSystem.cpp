#include "CoroutineSystem.h"

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

void leap::CoroutineSystem::CheckResume()
{
	for (auto& copair : m_Coroutines)
	{
		copair.pIEnumerator->Resume();
		if (copair.pIEnumerator->Value() || copair.pIEnumerator->IsDone())
		{
			copair.coroutine->Resume();
			if (!copair.coroutine->IsDone())
			{
				copair.pIEnumerator = std::make_unique<IEnumerator>(copair.coroutine->Value());
			}
		}
	}
}
