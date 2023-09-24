#pragma once

#include "../Component.h"
#include "Interfaces/IMeshRenderer.h"

namespace leap
{
	namespace graphics
	{
		class IMaterial;
	}

	class MeshRendererComponent final : public Component
	{
	public:
		MeshRendererComponent();
		~MeshRendererComponent() = default;

		MeshRendererComponent(const MeshRendererComponent& other) = delete;
		MeshRendererComponent(MeshRendererComponent&& other) = delete;
		MeshRendererComponent& operator=(const MeshRendererComponent& other) = delete;
		MeshRendererComponent& operator=(MeshRendererComponent&& other) = delete;

		void SetMaterial(graphics::IMaterial* pMaterial);

	protected:
		virtual void LateUpdate() override;
		virtual void OnDestroy() override;

	private:
		graphics::IMeshRenderer* m_pRenderer{};
	};
}