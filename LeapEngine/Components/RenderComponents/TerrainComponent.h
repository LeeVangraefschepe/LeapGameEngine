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
		TerrainComponent();
		virtual ~TerrainComponent() = default;

		TerrainComponent(const TerrainComponent& other) = delete;
		TerrainComponent(TerrainComponent&& other) = delete;
		TerrainComponent& operator=(const TerrainComponent& other) = delete;
		TerrainComponent& operator=(TerrainComponent&& other) = delete;

		std::vector<float>& GetHeights() { return m_Heights; }
		void ApplyHeights();

	private:
		virtual void Awake() override;

		graphics::ITexture* m_pTexture{};
		std::vector<float> m_Heights{};

		unsigned int m_Size{ 1024 };
	};
}