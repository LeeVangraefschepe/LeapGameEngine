#pragma once

#include "../Component.h"

#include <vector>

namespace leap
{
	namespace graphics
	{
		class ITexture;
		class IMaterial;
	}

	class TerrainComponent final : public Component
	{
	public:
		TerrainComponent() = default;
		virtual ~TerrainComponent() = default;

		TerrainComponent(const TerrainComponent& other) = delete;
		TerrainComponent(TerrainComponent&& other) = delete;
		TerrainComponent& operator=(const TerrainComponent& other) = delete;
		TerrainComponent& operator=(TerrainComponent&& other) = delete;

		void SetSize(unsigned int size);
		void SetTexture(graphics::ITexture* pTexture);

		std::vector<float>& GetHeights();
		void ApplyHeights();

	private:
		void ApplyTexture() const;
		void Init();

		graphics::ITexture* m_pTexture{};
		std::vector<float> m_Heights{};

		unsigned int m_Size{};

		bool m_IsInitialized{};
	};
}