#pragma once

#include "../Component.h"

#include <Interfaces/IMeshRenderer.h>
#include <Observer.h>

namespace leap
{
	class Mesh;
	namespace graphics
	{
		class IMaterial;
		class CustomMesh;
	}

	class MeshRenderer final : public Component, public Observer
	{
	public:
		MeshRenderer();
		~MeshRenderer() = default;

		MeshRenderer(const MeshRenderer& other) = delete;
		MeshRenderer(MeshRenderer&& other) = delete;
		MeshRenderer& operator=(const MeshRenderer& other) = delete;
		MeshRenderer& operator=(MeshRenderer&& other) = delete;

		void SetMesh(const Mesh& mesh);
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