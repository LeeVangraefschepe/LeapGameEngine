#include "TerrainComponent.h"

#include "MeshRenderer.h"

#include "../../SceneGraph/GameObject.h"
#include "../../ServiceLocator/ServiceLocator.h"

#include <Interfaces/IRenderer.h>

#include <Data/CustomMesh.h>
#include <Shaders/Heightmap.h>
#include <Shaders/Pos3D.h>

#include <Interfaces/IMaterial.h>
#include <Interfaces/ITexture.h>

#include <Data/CustomMesh.h>

#include <Debug.h>

void leap::TerrainComponent::SetTexture(leap::graphics::ITexture* pTexture)
{
	// Cache terrain size
	m_Size = pTexture->GetSize().x;

	// Cache the texture
	m_pTexture = pTexture;

	// Initialize the terrain mesh
	Init();

	// Apply the texture to the mesh
	ApplyTexture();
}

void leap::TerrainComponent::SetSize(unsigned int size)
{
	// Cache terrain size
	m_Size = size;

	// Create height information
	m_Heights.resize(static_cast<size_t>(m_Size * m_Size));

	// Create texture
	m_pTexture = ServiceLocator::GetRenderer().CreateTexture(m_Size, m_Size);

	// Initialize the terrain mesh
	Init();

	// Apply the texture to the mesh
	ApplyTexture();
}

std::vector<float>& leap::TerrainComponent::GetHeights()
{
	// Initialize the terrain mesh
	Init();

	return m_Heights;
}

void leap::TerrainComponent::ApplyHeights()
{
	m_pTexture->SetData(m_Heights.data(), static_cast<unsigned int>(m_Heights.size() * sizeof(float)));
}

void leap::TerrainComponent::ApplyTexture() const
{
	// Apply texture
	GetGameObject()->GetComponent<MeshRendererComponent>()->GetMaterial()->SetTexture("gHeightMap", m_pTexture);
}

void leap::TerrainComponent::Init()
{
	if (m_IsInitialized) return;

	// Set a default size, if none is applied
	if (m_Size == 0)
	{
		constexpr unsigned int defaultSize{ 1000 };
		SetSize(defaultSize);
	}

	m_IsInitialized = true;

	MeshRendererComponent* pMeshRenderer{ GetGameObject()->GetComponent<MeshRendererComponent>() };

	// Create mesh
	graphics::CustomMesh plane{};

	constexpr unsigned int vertexSize{ sizeof(leap::graphics::VertexPosUV) };

	for (unsigned int x{}; x < m_Size - 1; ++x)
	{
		for (unsigned int y{}; y < m_Size - 1; ++y)
		{
			const unsigned int startIdx{ static_cast<unsigned int>(plane.GetVertexBuffer().size()) / vertexSize };

			plane.AddVertex(leap::graphics::VertexPosUV{ { x, 0.0f, y + 1.0f }, { x, y + 1.0f } });
			plane.AddVertex(leap::graphics::VertexPosUV{ { x + 1.0f, 0.0f, y + 1.0f }, { x + 1.0f, y + 1.0f } });
			plane.AddVertex(leap::graphics::VertexPosUV{ { x + 1.0f, 0.0f, y }, { x + 1.0f, y } });
			plane.AddVertex(leap::graphics::VertexPosUV{ { x, 0.0f, y }, { x, y } });

			plane.AddIndex(startIdx);
			plane.AddIndex(startIdx + 1);
			plane.AddIndex(startIdx + 2);

			plane.AddIndex(startIdx);
			plane.AddIndex(startIdx + 2);
			plane.AddIndex(startIdx + 3);
		}
	}

	pMeshRenderer->LoadMesh(plane);

	// Create material
	graphics::IMaterial* pMaterial{ ServiceLocator::GetRenderer().CreateMaterial(graphics::shaders::Heightmap::GetShader(), "Heightmap") };
	pMeshRenderer->SetMaterial(pMaterial);
}