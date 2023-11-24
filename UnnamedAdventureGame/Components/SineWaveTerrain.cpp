#include "SineWaveTerrain.h"

#include <Components/RenderComponents/TerrainComponent.h>
#include <SceneGraph/GameObject.h>
#include <Components/Transform/Transform.h>

void unag::SineWaveTerrain::Awake()
{
	leap::TerrainComponent* pTerrain{ GetGameObject()->GetComponent<leap::TerrainComponent>() };

	auto heights{ pTerrain->GetHeights() };

	for (size_t x{}; x < 1025; ++x)
	{
		float height{ sinf(x / 50.0f) * 0.5f + 0.5f };
		for (size_t y{}; y < 1025; ++y)
		{
			heights[x + y * 1025] = height;
		}
	}

	pTerrain->SetHeights(heights);
}