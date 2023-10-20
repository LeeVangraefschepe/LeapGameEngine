#pragma once

#include "../../Component.h"

#include <vector>
#include <memory>

#include <Command.h>

#include <vec2.hpp>

namespace leap
{
	class ICanvasElement;

	class CanvasActions final : public Component
	{
	public:
		CanvasActions() = default;
		virtual ~CanvasActions() = default;

		CanvasActions(const CanvasActions& other) = delete;
		CanvasActions(CanvasActions&& other) = delete;
		CanvasActions& operator=(const CanvasActions& other) = delete;
		CanvasActions& operator=(CanvasActions&& other) = delete;

		void Add(ICanvasElement* pElement);
		void Remove(ICanvasElement* pElement);

	protected:
		virtual void Awake() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void OnDestroy() override;

	private:
		void OnClickStart();
		void OnClick();
		void OnClickEnd();
		void OnMove();

		void SortOnDepth();
		bool IsPosInElement(const glm::vec2& position, ICanvasElement* pElement);
		glm::ivec2 GetMousePosition() const;
		glm::ivec2 GetMouseDelta() const;

		bool m_IsDirty{};

		std::vector<std::unique_ptr<Command>> m_pCommands{};

		std::vector<ICanvasElement*> m_pElements{};
		ICanvasElement* m_pClickingElement{};
	};
}