#include "CanvasElement.h"

#include "../../../SceneGraph/GameObject.h"
#include "../../Transform/Transform.h"
#include "CanvasComponent.h"
#include "../SpriteRendererComponent.h"

void leap::CanvasElement::SetSize(const glm::vec2& size)
{
	SetSize(size.x, size.y);
}

void leap::CanvasElement::SetSize(float x, float y)
{
	GetTransform()->SetWorldScale(x, y, 1.0f);
	SetDirty(DirtyFlag::Resize, true);
}

void leap::CanvasElement::SetSize(float size)
{
	SetSize(size, size);
}

void leap::CanvasElement::SetNativeSize()
{
	SetDirty(DirtyFlag::NativeSize, true);
}

void leap::CanvasElement::Awake()
{
	if (IsDirty(DirtyFlag::NativeSize))
	{
		SetDirty(DirtyFlag::NativeSize, false);

		InternalSetNativeSize();
	}

	if (!IsDirty(DirtyFlag::Resize)) return;

	SetDirty(DirtyFlag::Resize, false);

	CanvasComponent* pCanvas{ GetGameObject()->GetParent()->GetComponent<CanvasComponent>() };

	const glm::ivec2& reference{ pCanvas->GetReference() };
	const glm::vec2& canvasSize{ pCanvas->GetTransform()->GetWorldScale() };

	const glm::vec2& multiplier{ canvasSize.x / reference.x, canvasSize.y / reference.y };
	const glm::vec2& elementSize{ GetTransform()->GetWorldScale() };

	GetTransform()->SetWorldScale(multiplier.x * elementSize.x, multiplier.y * elementSize.y, 1.0f);
}

void leap::CanvasElement::InternalSetNativeSize()
{
	SpriteRendererComponent* pSprite{ GetGameObject()->GetComponent<SpriteRendererComponent>() };
	
	const glm::vec2 nativeSize{ pSprite->GetTexture()->GetSize() };
	SetSize(nativeSize);
}

bool leap::CanvasElement::IsDirty(DirtyFlag dirtyFlag) const
{
	return static_cast<bool>(m_DirtyFlag & static_cast<unsigned int>(dirtyFlag));
}

void leap::CanvasElement::SetDirty(DirtyFlag dirtyFlag, bool enabled)
{
	if (enabled)
	{
		m_DirtyFlag |= static_cast<unsigned int>(dirtyFlag);
	}
	else
	{
		m_DirtyFlag &= ~static_cast<unsigned int>(dirtyFlag);
	}
}
