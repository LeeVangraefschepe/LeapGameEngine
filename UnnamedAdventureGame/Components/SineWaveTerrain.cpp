#include "SineWaveTerrain.h"

#include <Components/RenderComponents/TerrainComponent.h>
#include <SceneGraph/GameObject.h>
#include <Components/Transform/Transform.h>

void unag::SineWaveTerrain::Awake()
{
	/*leap::TerrainComponent* pTerrain{ GetGameObject()->GetComponent<leap::TerrainComponent>() };

	auto& heights{ pTerrain->GetHeights() };

	for (size_t x{}; x < 1000; ++x)
	{
		float height{ (GetTransform()->GetWorldPosition().x + x + 1000.0f) / 2000.0f };
		for (size_t y{}; y < 1000; ++y)
		{
			heights[x + y * 1000] = height;
		}
	}

	pTerrain->ApplyHeights();*/
}
