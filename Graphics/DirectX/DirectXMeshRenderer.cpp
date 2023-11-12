#include "DirectXMeshRenderer.h"
#include <d3d11.h>
#include <d3dx11effect.h>

#include "Debug.h"
#include "DirectXMaterial.h"
#include "DirectXMeshLoader.h"
#include "DirectXDefaults.h"

#include "../MeshLoader.h"

leap::graphics::DirectXMeshRenderer::DirectXMeshRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice{ pDevice }
	, m_pDeviceContext{ pDeviceContext }
{
}

leap::graphics::DirectXMeshRenderer::~DirectXMeshRenderer()
{
}

void leap::graphics::DirectXMeshRenderer::Draw()
{
	Draw(m_pMaterial);
}

void leap::graphics::DirectXMeshRenderer::Draw(IMaterial* pMaterial)
{
	unsigned int vertexSize{ m_VertexSize };
	ID3D11Buffer* pVertexBuffer{ m_pVertexBuffer };
	ID3D11Buffer* pIndexBuffer{ m_pIndexBuffer };
	unsigned int nrIndices{ m_NrIndices };

	if (!pMaterial) pMaterial = DirectXDefaults::GetInstance().GetMaterialNotFound(m_pDevice);
	if (!m_pVertexBuffer || !m_pIndexBuffer)
	{
		if (m_IsLineRenderer) return;

		DirectXDefaults& defaults{ DirectXDefaults::GetInstance() };
		pMaterial = defaults.GetMaterialError(m_pDevice);
		defaults.GetMeshError(m_pDevice, vertexSize, pVertexBuffer, pIndexBuffer, nrIndices);
	}

	DirectXMaterial* pDXMaterial{ static_cast<DirectXMaterial*>(pMaterial) };

	// Apply the world transformation
	pDXMaterial->SetWorldMatrix(m_Transform);

	// Set primitive topology
	m_pDeviceContext->IASetPrimitiveTopology(m_IsLineRenderer ? D3D11_PRIMITIVE_TOPOLOGY_LINELIST : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set input layout
	m_pDeviceContext->IASetInputLayout(pDXMaterial->GetInputLayout());

	// Set vertex buffer
	const UINT stride{ vertexSize };
	constexpr UINT offset{ 0 };
	m_pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Set index buffer
	m_pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	D3DX11_TECHNIQUE_DESC techniqueDesc{};
	HRESULT result{ pDXMaterial->GetTechnique()->GetDesc(&techniqueDesc) };
	if (FAILED(result)) Debug::LogError("DirectXRenderer Error : Failed to get description of effect technique");

	for (UINT p{}; p < techniqueDesc.Passes; ++p)
	{
		result = pDXMaterial->GetTechnique()->GetPassByIndex(p)->Apply(0, m_pDeviceContext);
		if (FAILED(result)) Debug::LogError("DirectXRenderer Error : Failed to apply a effect technique pass to device");

		m_pDeviceContext->DrawIndexed(nrIndices, 0, 0);
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

void leap::graphics::DirectXMeshRenderer::SetTransform(const glm::mat4x4& transform)
{
	m_Transform = transform;
}

void leap::graphics::DirectXMeshRenderer::LoadMesh(const std::string& filePath)
{
	m_FilePath = filePath;

	const DirectXMeshLoader::DirectXMeshDefinition& mesh{ DirectXMeshLoader::GetInstance().LoadMesh(filePath, m_pDevice) };

	if (m_HasCustomMesh)
	{
		DirectXMeshLoader::GetInstance().RemoveCustomMesh(m_pVertexBuffer);
	}

	m_HasCustomMesh = false;
	m_VertexSize = mesh.vertexSize;
	m_pVertexBuffer = mesh.vertexBuffer;
	m_pIndexBuffer = mesh.indexBuffer;
	m_NrIndices = mesh.nrIndices;
}

void leap::graphics::DirectXMeshRenderer::LoadMesh(const CustomMesh& mesh)
{
	const DirectXMeshLoader::DirectXMeshDefinition& directXMesh{ DirectXMeshLoader::GetInstance().LoadMesh(mesh, m_pDevice) };

	if (m_HasCustomMesh)
	{
		DirectXMeshLoader::GetInstance().RemoveCustomMesh(m_pVertexBuffer);
	}

	m_HasCustomMesh = true;
	m_VertexSize = directXMesh.vertexSize;
	m_pVertexBuffer = directXMesh.vertexBuffer;
	m_pIndexBuffer = directXMesh.indexBuffer;
	m_NrIndices = directXMesh.nrIndices;
}

void leap::graphics::DirectXMeshRenderer::SetIsLineRenderer(bool isLineRenderer)
{
	m_IsLineRenderer = isLineRenderer;
}

void leap::graphics::DirectXMeshRenderer::Reload(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	LoadMesh(m_FilePath);

	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	SetMaterial(m_pMaterial);
}
