#include "CanvasActions.h"

#include "ICanvasElement.h"

#include <InputManager.h>
#include <Mouse.h>

#include <LambdaCommand.h>

#include "../../../GameContext/GameContext.h"
#include "../../../GameContext/Window.h"

#include <algorithm>

void leap::CanvasActions::Add(ICanvasElement* pElement)
{
	m_pElements.push_back(pElement);
}

void leap::CanvasActions::Remove(ICanvasElement* pElement)
{
	m_pElements.erase(std::remove(begin(m_pElements), end(m_pElements), pElement));
}

void leap::CanvasActions::Awake()
{
	m_pCommands.emplace_back(std::make_unique<LambdaCommand>([this]() { OnClickStart(); }));
	input::InputManager::GetInstance().GetMouse()->AddCommand(m_pCommands[m_pCommands.size() - 1].get(), input::InputManager::InputType::EventPress, input::Mouse::Button::LeftButton);

	m_pCommands.emplace_back(std::make_unique<LambdaCommand>([this]() { OnClick(); }));
	input::InputManager::GetInstance().GetMouse()->AddCommand(m_pCommands[m_pCommands.size() - 1].get(), input::InputManager::InputType::EventRepeat, input::Mouse::Button::LeftButton);

	m_pCommands.emplace_back(std::make_unique<LambdaCommand>([this]() { OnClickEnd(); }));
	input::InputManager::GetInstance().GetMouse()->AddCommand(m_pCommands[m_pCommands.size() - 1].get(), input::InputManager::InputType::EventRelease, input::Mouse::Button::LeftButton);
}

void leap::CanvasActions::Update()
{
	m_IsDirty = true;
}

void leap::CanvasActions::LateUpdate()
{
	const glm::ivec2& mouseDelta{ input::InputManager::GetInstance().GetMouse()->GetDelta() };
	if (abs(mouseDelta.x) > 0 || abs(mouseDelta.y) > 0)
	{
		OnMove();
	}
}

void leap::CanvasActions::OnDestroy()
{
	for (const auto& pCommand : m_pCommands)
	{
		input::InputManager::GetInstance().GetMouse()->RemoveCommand(pCommand.get());
	}
}

void leap::CanvasActions::OnClickStart()
{
	const glm::vec2 mousePos{ GetMousePosition() };

	if (m_IsDirty) SortOnDepth();

	for (auto it{ m_pElements.rbegin() }; it < m_pElements.rend(); ++it)
	{
		ICanvasElement* pElement{ *it };
		if (IsPosInElement(mousePos, pElement))
		{
			// Store the current clicking element
			m_pClickingElement = pElement;

			// Start clicking the element
			pElement->OnClickStart();

			// Don't check other elements, only one element can be clicked at a time
			return;
		}
	}
}

void leap::CanvasActions::OnClick()
{
	if (m_pClickingElement == nullptr) return;
	m_pClickingElement->OnClick();
}

void leap::CanvasActions::OnClickEnd()
{
	if (m_pClickingElement == nullptr) return;

	m_pClickingElement->OnClickEnd();
	m_pClickingElement = nullptr;
}

void leap::CanvasActions::OnMove()
{
	const glm::vec2 mousePos{ GetMousePosition() };
	const glm::vec2 prevMousePos{ GetMousePosition() - GetMouseDelta() };

	if (m_IsDirty) SortOnDepth();

	for (auto it{ m_pElements.rbegin() }; it < m_pElements.rend(); ++it)
	{
		ICanvasElement* pElement{ *it };
		if (!IsPosInElement(mousePos, pElement) && IsPosInElement(prevMousePos, pElement))
		{
			pElement->OnHoverExit();
			return;
		}
	}

	for (auto it{ m_pElements.rbegin() }; it < m_pElements.rend(); ++it)
	{
		ICanvasElement* pElement{ *it };
		if (IsPosInElement(mousePos, pElement))
		{
			if (!IsPosInElement(prevMousePos, pElement))
			{
				pElement->OnHoverEnter();
			}

			pElement->OnHover();
			return;
		}
	}
}

void leap::CanvasActions::SortOnDepth()
{
	std::stable_sort(begin(m_pElements), end(m_pElements), [](ICanvasElement* pFirst, ICanvasElement* pSecond)
		{
			return pFirst->GetPosition().z < pSecond->GetPosition().z;
		});
}

bool leap::CanvasActions::IsPosInElement(const glm::vec2& position, ICanvasElement* pElement)
{
	const glm::vec2 pos{ pElement->GetPosition()};
	const glm::vec2 size{ pElement->GetSize() };
	const glm::vec2 pivot{ pElement->GetPivot() };

	const glm::vec2 center{ pos + (0.5f - pivot) * size };

	const glm::vec2 bottomLeft{ center - size / 2.0f };
	const glm::vec2 topRight{ center + size / 2.0f };

	if (position.x < bottomLeft.x || position.x > topRight.x) return false;
	if (position.y < bottomLeft.y || position.y > topRight.y) return false;

	return true;
}

glm::ivec2 leap::CanvasActions::GetMousePosition() const
{
	// Get the mouse position relative to the UI system (0,0 is middle of the screen for UI while 0,0 is top left corner for the mouse)
	glm::vec2 mousePos{ input::InputManager::GetInstance().GetMouse()->GetPos() - GameContext::GetInstance().GetWindow()->GetWindowSize() / 2 };
	// Invert the mouse since the UI system works with a Y-up system instead of the Y-down system from the mouse
	mousePos.y = -mousePos.y;

	return mousePos;
}

glm::ivec2 leap::CanvasActions::GetMouseDelta() const
{
	glm::ivec2 mouseDelta{ input::InputManager::GetInstance().GetMouse()->GetDelta() };
	// Invert the mouse since the UI system works with a Y-up system instead of the Y-down system from the mouse
	mouseDelta.y = -mouseDelta.y;

	return mouseDelta;
}
