#pragma once

namespace leap
{
	template<typename T>
	class GraphicsObject final
	{
	public:
		GraphicsObject() = default;
		GraphicsObject(T* pObject) : m_pObject{ pObject } {}
		~GraphicsObject()
		{
			Release();
		}

		GraphicsObject& operator=(const GraphicsObject& other) = delete;
		GraphicsObject(const GraphicsObject& other) = delete;
		GraphicsObject& operator=(GraphicsObject&& other) = delete;
		GraphicsObject(const GraphicsObject&& other) = delete;

		void Clear()
		{
			Release();
		}

		void SetObject(T* pObject)
		{
			if (pObject == m_pObject) return;

			Release();
			m_pObject = pObject;
		}

		T* GetObject()
		{
			++m_UseCounter;
			return m_pObject;
		}

		T* GetUncountedObject() const
		{
			return m_pObject;
		}

		bool IsValid() const
		{
			return m_pObject;
		}

	private:
		void Release()
		{
			if (m_pObject && !m_UseCounter) m_pObject->Remove();
			m_UseCounter = 0;
		}

		T* m_pObject{};
		unsigned int m_UseCounter{};
	};
}