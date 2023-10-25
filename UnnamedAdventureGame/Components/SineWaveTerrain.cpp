#include "SineWaveTerrain.h"

#include <Components/RenderComponents/TerrainComponent.h>
#include <SceneGraph/GameObject.h>

void unag::SineWaveTerrain::Awake()
{
	leap::TerrainComponent* pTerrain{ GetGameObject()->GetComponent<leap::TerrainComponent>() };

	auto& heights{ pTerrain->GetHeights() };

	for (size_t x{}; x < 1024; ++x)
	{
		float height = sinf(x / 50.0f) * 0.5f + 0.5f;
		for (size_t y{}; y < 1024; ++y)
		{
			heights[x + y * 1024] = height;
		}
	}

	pTerrain->ApplyHeights();
}
