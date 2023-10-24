#pragma once

#include "../Component.h"

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

	private:
		virtual void Awake() override;

		graphics::ITexture* m_pTexture{};
	};
}