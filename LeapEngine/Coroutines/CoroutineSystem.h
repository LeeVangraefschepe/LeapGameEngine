#pragma once
#include <vector>
#include <utility>
#include "Coroutine.h"

namespace leap
{
    class CoroutineSystem final
    {
    public:
        CoroutineSystem();
        ~CoroutineSystem();

        void Register(Coroutine<>* pCoroutine);
        void Unregister(Coroutine<>* pCoroutine);

    private:
        struct CoPair final
        {
            Coroutine<>* coroutine{};
            IEnumerator* ienumerator{};
        };
        std::vector<CoPair> m_Coroutines;
    };
}