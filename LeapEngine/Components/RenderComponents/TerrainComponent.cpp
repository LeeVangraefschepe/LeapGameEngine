#include "TerrainComponent.h"

#include <Interfaces/ITexture.h>
#include <Interfaces/IRenderer.h>
#include <Interfaces/IMeshRenderer.h>
#include <Data/Vertex.h>
#include <Shaders/Heightmap.h>

#include "../../ServiceLocator/ServiceLocator.h"
#include "../Transform/Transform.h"

#include "../../Graphics/Shader.h"

#include <sstream>

leap::TerrainComponent::TerrainComponent()
{
	m_pRenderer = ServiceLocator::GetRenderer().CreateMeshRenderer();

	m_pMaterial = Material::CreatePtr<graphics::shaders::Heightmap>("Heightmap", true);

	m_pRenderer->SetMaterial(m_pMaterial->GetInternal());
}

leap::TerrainComponent::~TerrainComponent()
{
	auto& meshData{ m_Meshes[m_Size] };
	--meshData.useCounter;
	if (meshData.useCounter == 0)
	{
		m_Meshes.erase(m_Size);
	}
	else
	{
		m_pRenderer->UnsetMesh();
	}
	ServiceLocator::GetRenderer().RemoveMeshRenderer(m_pRenderer);
}

void leap::TerrainComponent::SetSize(unsigned int size)
{
	if (m_Size == size) return;

	unsigned int prevSize{ m_Size };
	m_Size = size;

	ApplySizeTexture();
	ApplySizeMesh(prevSize);
}

void leap::TerrainComponent::LoadHeightmap(const std::string& path, bool isShared)
{
	m_Texture.Load(path, !isShared);

	const auto& textureSize{ m_Texture.GetSize() };
	if (textureSize.x != textureSize.y)
	{
		Debug::LogError("LeapEngine Error : Cannot create terrain with a texture which different width and height values");
		return;
	}

	m_pMaterial->Set("gHeightMap", m_Texture);

	const unsigned int meshSize{ textureSize.x - 1 };
	if (m_Size == meshSize) return;

	const unsigned int prevSize{ m_Size };
	m_Size = meshSize;
	ApplySizeMesh(prevSize);
}

std::vector<float> leap::TerrainComponent::GetHeights() const
{
	std::vector<float> heights{};
	const std::vector<unsigned char> textureData{ m_Texture.GetData() };
	heights.resize(textureData.size() / sizeof(float));
	for (size_t i{}; i < heights.size(); ++i)
	{
		const unsigned char byte{ textureData[i * sizeof(float)]};
		const float height{ byte / 255.0f };
		heights[i] = height;
	}
	return heights;
}

void leap::TerrainComponent::SetHeights(const std::vector<float>& heights)
{
	std::vector<unsigned char> textureData{};
	textureData.resize(heights.size() * sizeof(float));
	for (size_t i{}; i < heights.size(); ++i)
	{
		const float height{ heights[i] };
		const unsigned char byte{ static_cast<unsigned char>(height * UCHAR_MAX) };
		const size_t byteIdx{ i * sizeof(float) };
		textureData[byteIdx] = byte;
		textureData[byteIdx + 1] = byte;
		textureData[byteIdx + 2] = byte;
		textureData[byteIdx + 3] = UCHAR_MAX; // Alpha channel
	}
	m_Texture.SetData(textureData);
}

void leap::TerrainComponent::Awake()
{
	m_pRenderer->SetTransform(GetTransform()->GetWorldTransform());
}

void leap::TerrainComponent::ApplySizeTexture()
{
	m_Texture.Load(m_Size + 1, m_Size + 1);
	m_pMaterial->Set("gHeightMap", m_Texture);
}

void leap::TerrainComponent::ApplySizeMesh(unsigned int prevSize)
{
	if (!m_Meshes.contains(m_Size)) CreateMesh(m_Size);

	TerrainMesh& mesh{ m_Meshes[m_Size] };
	++mesh.useCounter;

	TerrainMesh& prevMesh{ m_Meshes[prevSize] };
	--prevMesh.useCounter;
	if (prevMesh.useCounter <= 0) m_Meshes.erase(prevSize);

	m_pRenderer->SetMesh(mesh.mesh.GetInternal());
}

void leap::TerrainComponent::CreateMesh(unsigned int size)
{
	Mesh mesh{};

	mesh.SetWritable(true);

	std::vector<leap::graphics::VertexPosUV> vertices{};
	std::vector<unsigned> indices{};

	for (unsigned int x{}; x < size; ++x)
	{
		for (unsigned int y{}; y < size; ++y)
		{
			const unsigned int startIdx{ static_cast<unsigned>(vertices.size()) };

			vertices.emplace_back(leap::graphics::VertexPosUV{ { x, 0.0f, y + 1.0f }, { x, y + 1.0f } });
			vertices.emplace_back(leap::graphics::VertexPosUV{ { x + 1.0f, 0.0f, y + 1.0f }, { x + 1.0f, y + 1.0f } });
			vertices.emplace_back(leap::graphics::VertexPosUV{ { x + 1.0f, 0.0f, y }, { x + 1.0f, y } });
			vertices.emplace_back(leap::graphics::VertexPosUV{ { x, 0.0f, y }, { x, y } });

			indices.emplace_back(startIdx);
			indices.emplace_back(startIdx + 1);
			indices.emplace_back(startIdx + 2);

			indices.emplace_back(startIdx);
			indices.emplace_back(startIdx + 2);
			indices.emplace_back(startIdx + 3);
		}
	}

	mesh.SetData(vertices, std::move(indices));

	m_Meshes[size] = TerrainMesh{ 0, std::move(mesh) };
}
