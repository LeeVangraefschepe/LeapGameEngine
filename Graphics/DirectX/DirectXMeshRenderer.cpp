#include "DirectXMeshRenderer.h"
#include <d3d11.h>
#include <d3dx11effect.h>

#include "Debug.h"
#include "DirectXMaterial.h"
#include "DirectXDefaults.h"
#include "DirectXMesh.h"
#include "DirectXEngine.h"

#include "../MeshLoader.h"

leap::graphics::DirectXMeshRenderer::DirectXMeshRenderer(DirectXEngine* pEngine)
	: m_pEngine{ pEngine }
{
}

leap::graphics::DirectXMeshRenderer::~DirectXMeshRenderer()
{
	if(m_pMesh) m_pMesh->Remove();
	if (m_pMaterial) m_pMaterial->Remove();
}

void leap::graphics::DirectXMeshRenderer::Draw()
{
	Draw(m_pMaterial);
}

void leap::graphics::DirectXMeshRenderer::Draw(IMaterial* pMaterial)
{	
	DirectXMesh* pRenderingMesh{ m_pMesh };

	if (m_TopologyType == D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED)
	{
		Debug::LogError("Unknown topology type detected when rendering DirectX mesh");
		return;
	}

	if (!pRenderingMesh)
	{
		if (m_TopologyType != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) return;

		DirectXDefaults& defaults{ DirectXDefaults::GetInstance() };
		pRenderingMesh = defaults.GetMeshError(m_pEngine);

		IMaterial* pErrorMaterial{ defaults.GetMaterialError(m_pEngine) };
		pMaterial = pErrorMaterial;
	}

	if (!pMaterial) pMaterial = DirectXDefaults::GetInstance().GetMaterialNotFound(m_pEngine);

	for (int i{}; i < pRenderingMesh->m_Buffers.size(); ++i)
	{
		const auto& buffers = pRenderingMesh->m_Buffers[i];
		DrawMesh(buffers, pMaterial);
	}
}

void leap::graphics::DirectXMeshRenderer::Draw(const std::vector<IMaterial*>& pMaterials)
{
	if (m_TopologyType == D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED)
	{
		Debug::LogError("Unknown topology type detected when rendering DirectX mesh");
		return;
	}

	if (!m_pMesh)
	{
		if (m_TopologyType != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) return;

		DirectXDefaults& defaults{ DirectXDefaults::GetInstance() };
		const auto& errorMeshBuffers{ defaults.GetMeshError(m_pEngine)->m_Buffers };
		IMaterial* pErrorMaterial{ defaults.GetMaterialError(m_pEngine) };

		for(const auto& buffer : errorMeshBuffers)
			DrawMesh(buffer, pErrorMaterial);

		return;
	}

	std::vector<IMaterial*> pAdditionalMaterials{};
	if (pMaterials.empty() || pMaterials.size() < m_pMesh->m_Buffers.size())
	{
		for (size_t i{ pMaterials.size() }; i < m_pMesh->m_Buffers.size(); ++i)
		{
			pAdditionalMaterials.emplace_back(DirectXDefaults::GetInstance().GetMaterialNotFound(m_pEngine));
		}
	}

	for (int i{}; i < m_pMesh->m_Buffers.size(); ++i)
	{
		const auto& buffers = m_pMesh->m_Buffers[i];
		IMaterial* pMaterial{ pMaterials.size() >= i + 1 ? pMaterials[i] : pAdditionalMaterials[pMaterials.size() - i] };

		DrawMesh(buffers, pMaterial);
	}
}

leap::graphics::IMaterial* leap::graphics::DirectXMeshRenderer::GetMaterial()
{
	return m_pMaterial;
}

void leap::graphics::DirectXMeshRenderer::SetMaterial(IMaterial* pMaterial)
{
	m_pMaterial = static_cast<DirectXMaterial*>(pMaterial);
}

void leap::graphics::DirectXMeshRenderer::UnsetMaterial()
{
	m_pMaterial = nullptr;
}

void leap::graphics::DirectXMeshRenderer::SetTransform(const glm::mat4x4& transform)
{
	m_Transform = transform;
}

void leap::graphics::DirectXMeshRenderer::SetMesh(IMesh* pMesh)
{
	if (m_pMesh == pMesh) return;

	if (m_pMesh) m_pMesh->Remove();

	m_pMesh = static_cast<DirectXMesh*>(pMesh);
}

leap::graphics::IMesh* leap::graphics::DirectXMeshRenderer::GetMesh()
{
	return m_pMesh;
}

void leap::graphics::DirectXMeshRenderer::UnsetMesh()
{
	m_pMesh = nullptr;
}

void leap::graphics::DirectXMeshRenderer::SetAsPointRenderer()
{
	m_TopologyType = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
}

void leap::graphics::DirectXMeshRenderer::SetAsLineRenderer()
{
	m_TopologyType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}

void leap::graphics::DirectXMeshRenderer::SetAsTriangleRenderer()
{
	m_TopologyType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void leap::graphics::DirectXMeshRenderer::DrawMesh(const DirectXMesh::DirectXBuffers& buffers, IMaterial* pMaterial)
{
	DirectXMaterial* pDXMaterial{ static_cast<DirectXMaterial*>(pMaterial) };

	// Apply the world transformation
	pDXMaterial->SetWorldMatrix(m_Transform);

	// Set primitive topology
	m_pEngine->GetContext()->IASetPrimitiveTopology(m_TopologyType);

	// Set input layout
	m_pEngine->GetContext()->IASetInputLayout(pDXMaterial->GetInputLayout());

	// Set vertex buffer
	const UINT stride{ buffers.m_VertexSize };
	constexpr UINT offset{ 0 };
	m_pEngine->GetContext()->IASetVertexBuffers(0, 1, &buffers.m_pVertexBuffer, &stride, &offset);

	// Set index buffer
	m_pEngine->GetContext()->IASetIndexBuffer(buffers.m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	D3DX11_TECHNIQUE_DESC techniqueDesc{};
	HRESULT result{ pDXMaterial->GetTechnique()->GetDesc(&techniqueDesc) };
	if (FAILED(result)) Debug::LogError("DirectXRenderer Error : Failed to get description of effect technique");

	for (UINT p{}; p < techniqueDesc.Passes; ++p)
	{
		result = pDXMaterial->GetTechnique()->GetPassByIndex(p)->Apply(0, m_pEngine->GetContext());
		if (FAILED(result)) Debug::LogError("DirectXRenderer Error : Failed to apply a effect technique pass to device");

		m_pEngine->GetContext()->DrawIndexed(buffers.m_NrIndices, 0, 0);
	}
}