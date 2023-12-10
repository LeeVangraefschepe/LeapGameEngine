#pragma once

#include "../Component.h"

#include <Interfaces/IMeshRenderer.h>

namespace leap
{
	class Mesh;
	class Material;
	namespace graphics
	{
		class IMaterial;
		class CustomMesh;
	}

	class MeshRenderer final : public Component
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer() = default;

		MeshRenderer(const MeshRenderer& other) = delete;
		MeshRenderer(MeshRenderer&& other) = delete;
		MeshRenderer& operator=(const MeshRenderer& other) = delete;
		MeshRenderer& operator=(MeshRenderer&& other) = delete;

		void SetMesh(const Mesh& mesh);
		void SetMaterial(const Material& material);
		graphics::IMaterial* GetMaterial() const;

	private:
		virtual void Awake() override;
		virtual void LateUpdate() override;
		virtual void OnDestroy() override;
		void OnTransformChanged();

		graphics::IMeshRenderer* m_pRenderer{};
		bool m_IsDirty{ true };
	};
}