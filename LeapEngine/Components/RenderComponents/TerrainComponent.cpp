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

//#include "TerrainComponent.h"
//
//#include "MeshRenderer.h"
//
//#include "../../SceneGraph/GameObject.h"
//#include "../../ServiceLocator/ServiceLocator.h"
//
//#include <Interfaces/IRenderer.h>
//
//#include <Data/CustomMesh.h>
//#include <Shaders/Heightmap.h>
//#include <Shaders/Pos3D.h>
//
//#include <Interfaces/IMaterial.h>
//#include <Interfaces/ITexture.h>
//
//#include "../../Graphics/Mesh.h"
//
//#include <Data/Vertex.h>
//
//#include <Debug.h>
//
//#include <sstream>
//
//void leap::TerrainComponent::SetTexture(leap::graphics::ITexture* pTexture)
//{
//	// Cache terrain size
//	m_Size = pTexture->GetSize().x;
//
//	// Cache the texture
//	m_pTexture = pTexture;
//
//	// Initialize the terrain mesh
//	Init();
//
//	// Apply the texture to the mesh
//	ApplyTexture();
//}
//
//void leap::TerrainComponent::SetSize(unsigned int size)
//{
//	// Cache terrain size
//	m_Size = size;
//
//	const unsigned int sizeInVertices{ m_Size + 1 };
//
//	// Create height information
//	m_Heights.resize(static_cast<size_t>(sizeInVertices * sizeInVertices));
//
//	// Create texture
//	m_pTexture = ServiceLocator::GetRenderer().CreateTexture(sizeInVertices, sizeInVertices);
//
//	// Initialize the terrain mesh
//	Init();
//
//	// Apply the texture to the mesh
//	ApplyTexture();
//}
//
//std::vector<float>& leap::TerrainComponent::GetHeights()
//{
//	// Initialize the terrain mesh
//	Init();
//
//	return m_Heights;
//}
//
//void leap::TerrainComponent::ApplyHeights()
//{
//	m_pTexture->SetData(m_Heights.data(), static_cast<unsigned int>(m_Heights.size() * sizeof(float)));
//
//	CheckExistenceMeshRenderer();
//	ApplyTexture();
//}
//
//void leap::TerrainComponent::ApplyTexture()
//{
//	// Apply texture
//	m_pMeshRenderer->GetMaterial()->SetTexture("gHeightMap", m_pTexture);
//}
//
//void leap::TerrainComponent::Init()
//{
//	if (m_IsInitialized) return;
//
//	// Set a default size, if none is applied
//	if (m_Size == 0)
//	{
//		constexpr unsigned int defaultSize{ 1000 };
//		SetSize(defaultSize);
//	}
//
//	m_IsInitialized = true;
//
//	CheckExistenceMeshRenderer();
//	ApplyTexture();
//
//	// Create mesh
//	static Mesh plane{};
//	if (!m_MeshInitialized)
//	{
//		m_MeshInitialized = true;
//
//		plane.SetWritable(true);
//
//		std::vector<leap::graphics::VertexPosUV> vertices{};
//		std::vector<unsigned> indices{};
//
//		for (unsigned int x{}; x < m_Size; ++x)
//		{
//			for (unsigned int y{}; y < m_Size; ++y)
//			{
//				const unsigned int startIdx{ static_cast<unsigned>(vertices.size()) };
//
//				vertices.emplace_back(leap::graphics::VertexPosUV{ { x, 0.0f, y + 1.0f }, { x, y + 1.0f } });
//				vertices.emplace_back(leap::graphics::VertexPosUV{ { x + 1.0f, 0.0f, y + 1.0f }, { x + 1.0f, y + 1.0f } });
//				vertices.emplace_back(leap::graphics::VertexPosUV{ { x + 1.0f, 0.0f, y }, { x + 1.0f, y } });
//				vertices.emplace_back(leap::graphics::VertexPosUV{ { x, 0.0f, y }, { x, y } });
//
//				indices.emplace_back(startIdx);
//				indices.emplace_back(startIdx + 1);
//				indices.emplace_back(startIdx + 2);
//
//				indices.emplace_back(startIdx);
//				indices.emplace_back(startIdx + 2);
//				indices.emplace_back(startIdx + 3);
//			}
//		}
//
//		plane.SetData(vertices, std::move(indices));
//	}
//
//	m_pMeshRenderer->SetMesh(plane);
//}
//
//void leap::TerrainComponent::CheckExistenceMeshRenderer()
//{
//	if (!m_pMeshRenderer)
//	{
//		m_pMeshRenderer = GetGameObject()->AddComponent<MeshRenderer>();
//
//		// Create material
//		std::stringstream ss{};
//		ss << "Heightmap" << this;
//		graphics::IMaterial* pMaterial{ ServiceLocator::GetRenderer().CreateMaterial(graphics::shaders::Heightmap::GetShader(), ss.str()) };
//		m_pMeshRenderer->SetMaterial(pMaterial);
//	}
//}

leap::TerrainComponent::TerrainComponent()
{
	m_pRenderer = ServiceLocator::GetRenderer().CreateMeshRenderer();

	std::stringstream ss{};
	ss << "Heightmap" << this;
	m_pMaterial = std::make_unique<Material>(ss.str(), Shader<graphics::shaders::Heightmap>(), true);

	m_pRenderer->SetMaterial(m_pMaterial->GetInternal());

	m_Texture.OnInternalChange.AddListener(this);
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

void leap::TerrainComponent::Notify()
{
	const auto& textureSize{ m_Texture.GetSize() };
	if (textureSize.x != textureSize.y)
	{
		Debug::LogError("LeapEngine Error : Cannot create terrain with a texture which different width and height values");
		return;
	}

	m_pMaterial->Set("gHeightMap", m_Texture);

	if (m_Size == textureSize.x) return;

	unsigned int prevSize{ m_Size };
	m_Size = textureSize.x;
	ApplySizeMesh(prevSize);
}

void leap::TerrainComponent::Awake()
{
	m_pRenderer->SetTransform(GetTransform()->GetWorldTransform());
}

void leap::TerrainComponent::ApplySizeTexture()
{
	m_Texture.Load(m_Size, m_Size);
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
