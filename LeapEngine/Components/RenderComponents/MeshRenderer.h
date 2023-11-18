#pragma once

#include "../Component.h"
#include "Interfaces/IMeshRenderer.h"
#include <Observer.h>

namespace leap
{
	namespace graphics
	{
		class IMaterial;
		class CustomMesh;
	}

	class MeshRendererComponent final : public Component, public Observer
	{
	public:
		MeshRendererComponent();
		~MeshRendererComponent() = default;

		MeshRendererComponent(const MeshRendererComponent& other) = delete;
		MeshRendererComponent(MeshRendererComponent&& other) = delete;
		MeshRendererComponent& operator=(const MeshRendererComponent& other) = delete;
		MeshRendererComponent& operator=(MeshRendererComponent&& other) = delete;

		void LoadMesh(const std::string& filePath);
		void LoadMesh(const graphics::CustomMesh& mesh);
		void SetMaterial(graphics::IMaterial* pMaterial);
		graphics::IMaterial* GetMaterial() const;

	protected:
		virtual void Awake() override;
		virtual void LateUpdate() override;
		virtual void OnDestroy() override;
		virtual void Notify() override;

	private:
		graphics::IMeshRenderer* m_pRenderer{};
		bool m_IsDirty{ true };
	};
}