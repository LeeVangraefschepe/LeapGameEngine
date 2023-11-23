#include "DirectXMeshRenderer.h"
#include <d3d11.h>
#include <d3dx11effect.h>

#include "Debug.h"
#include "DirectXMaterial.h"
#include "DirectXMeshLoader.h"
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
	unsigned int vertexSize{};
	ID3D11Buffer* pVertexBuffer{};
	ID3D11Buffer* pIndexBuffer{};
	unsigned int nrIndices{};

	if (!m_pMesh)
	{
		if (m_IsLineRenderer) return;

		DirectXDefaults& defaults{ DirectXDefaults::GetInstance() };
		pMaterial = defaults.GetMaterialError(m_pEngine);
		defaults.GetMeshError(m_pEngine, vertexSize, pVertexBuffer, pIndexBuffer, nrIndices);
	}
	else
	{
		vertexSize = m_pMesh->m_VertexSize;
		pVertexBuffer = m_pMesh->m_pVertexBuffer;
		pIndexBuffer = m_pMesh->m_pIndexBuffer;
		nrIndices = m_pMesh->m_NrIndices;

		if (!pMaterial) pMaterial = DirectXDefaults::GetInstance().GetMaterialNotFound(m_pEngine);
	}

	DirectXMaterial* pDXMaterial{ static_cast<DirectXMaterial*>(pMaterial) };

	// Apply the world transformation
	pDXMaterial->SetWorldMatrix(m_Transform);

	// Set primitive topology
	m_pEngine->GetContext()->IASetPrimitiveTopology(m_IsLineRenderer ? D3D11_PRIMITIVE_TOPOLOGY_LINELIST : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set input layout
	m_pEngine->GetContext()->IASetInputLayout(pDXMaterial->GetInputLayout());

	// Set vertex buffer
	const UINT stride{ vertexSize };
	constexpr UINT offset{ 0 };
	m_pEngine->GetContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Set index buffer
	m_pEngine->GetContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	D3DX11_TECHNIQUE_DESC techniqueDesc{};
	HRESULT result{ pDXMaterial->GetTechnique()->GetDesc(&techniqueDesc) };
	if (FAILED(result)) Debug::LogError("DirectXRenderer Error : Failed to get description of effect technique");

	for (UINT p{}; p < techniqueDesc.Passes; ++p)
	{
		result = pDXMaterial->GetTechnique()->GetPassByIndex(p)->Apply(0, m_pEngine->GetContext());
		if (FAILED(result)) Debug::LogError("DirectXRenderer Error : Failed to apply a effect technique pass to device");

		m_pEngine->GetContext()->DrawIndexed(nrIndices, 0, 0);
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

void leap::graphics::DirectXMeshRenderer::UnsetMesh()
{
	m_pMesh = nullptr;
}

void leap::graphics::DirectXMeshRenderer::SetIsLineRenderer(bool isLineRenderer)
{
	m_IsLineRenderer = isLineRenderer;
}