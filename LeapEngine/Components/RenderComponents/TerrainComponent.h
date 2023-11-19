#pragma once
//
//#include "../Component.h"
//
//#include <vector>
//
//namespace leap
//{
//	class MeshRenderer;
//	namespace graphics
//	{
//		class ITexture;
//		class IMaterial;
//	}
//
//	class TerrainComponent final : public Component
//	{
//	public:
//		TerrainComponent() = default;
//		virtual ~TerrainComponent() = default;
//
//		TerrainComponent(const TerrainComponent& other) = delete;
//		TerrainComponent(TerrainComponent&& other) = delete;
//		TerrainComponent& operator=(const TerrainComponent& other) = delete;
//		TerrainComponent& operator=(TerrainComponent&& other) = delete;
//
//		/// <summary>
//		/// Set the size of the terrain in vertices. 
//		/// E.g. Given a size of 1000, a terrain of 1000x1000 vertices with 1 meter spacing between them gets created.
//		/// Do not call this function more then once, or you will fill your memory with unused textures
//		/// </summary>
//		void SetSize(unsigned int size);
//
//		/// <summary>
//		/// Set the heightmap texture for this terrain. A terrain mesh will be generated with the same size as the given texture.
//		/// E.g. Given a texture of size 1024x1024, a terrain of 1024x1024 vertices with 1 meter spacing between them gets created.
//		/// </summary>
//		void SetTexture(graphics::ITexture* pTexture);
//
//		/// <summary>
//		/// Get the internal height map.
//		/// This height map is only valid when the heightmap is initialized using the SetSize function.
//		/// Changes to this height map only have visual impact after calling TerrainComponent::ApplyHeights;
//		/// </summary>
//		std::vector<float>& GetHeights();
//
//		/// <summary>
//		/// Apply any changes made in the internal height map to the height map texture.
//		/// </summary>
//		void ApplyHeights();
//
//	private:
//		void ApplyTexture();
//		void Init();
//		void CheckExistenceMeshRenderer();
//
//		graphics::ITexture* m_pTexture{};
//		std::vector<float> m_Heights{};
//
//		MeshRenderer* m_pMeshRenderer{};
//
//		unsigned int m_Size{};
//
//		bool m_IsInitialized{};
//		inline static bool m_MeshInitialized{};
//	};
//}