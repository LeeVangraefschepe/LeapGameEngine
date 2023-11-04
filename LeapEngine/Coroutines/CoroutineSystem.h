#pragma once
#include <vector>
#include <utility>
#include <memory>
#include "Coroutine.h"

namespace leap
{
    class CoroutineSystem final
    {
    public:
        CoroutineSystem();
        ~CoroutineSystem();

        void Register(Coroutine<>* pCoroutine, IEnumerator&& iEnumerator);
        void Unregister(Coroutine<>* pCoroutine);

    private:
        struct CoPair final
        {
            Coroutine<>* coroutine{};
            std::unique_ptr<IEnumerator> pIEnumerator;
        };
        std::vector<CoPair> m_Coroutines;
    };
}