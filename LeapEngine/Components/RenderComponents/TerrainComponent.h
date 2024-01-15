#pragma once

#include "../Component.h"

#include "../../Graphics/Mesh.h"
#include "../../Graphics/Material.h"
#include "../../Graphics/Texture.h"

#include <vector>
#include <unordered_map>

namespace leap
{
	namespace graphics
	{
		class IMeshRenderer;
	}

	class TerrainComponent final : public Component
	{
	public:
		TerrainComponent();
		virtual ~TerrainComponent();

		TerrainComponent(const TerrainComponent& other) = delete;
		TerrainComponent(TerrainComponent&& other) = delete;
		TerrainComponent& operator=(const TerrainComponent& other) = delete;
		TerrainComponent& operator=(TerrainComponent&& other) = delete;

		void SetSize(unsigned int widthInQuads, float quadSize = 1.0f);
		void LoadHeightmap(const std::string& path, bool isShared = false, float quadSize = 1.0f);

		std::vector<float> GetHeights() const;
		void SetHeights(const std::vector<float>& heights);

	private:
		struct TerrainMesh final
		{
			unsigned int useCounter{ 1 };
			Mesh mesh{};
		};
		inline static std::unordered_map<unsigned int, TerrainMesh> m_Meshes{};
		std::unique_ptr<Material> m_pMaterial{};

		virtual void Awake() override;

		void UpdateTransform() const;
		void ApplySizeTexture();
		void ApplySizeMesh(unsigned int prevSize);
		void CreateMesh();

		unsigned int m_Size{ 256 };
		float m_QuadSize{ 1.0f };
		graphics::IMeshRenderer* m_pRenderer{};
		Texture m_Texture{};
	};
}