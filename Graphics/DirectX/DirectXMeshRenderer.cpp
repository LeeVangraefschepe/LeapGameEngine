#include "DirectXMeshRenderer.h"
#include <d3d11.h>
#include <d3dx11effect.h>

#include "DirectXMaterial.h"
#include "DirectXMeshLoader.h"

#include "../Utils.h"

leap::graphics::DirectXMeshRenderer::DirectXMeshRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice{ pDevice }
	, m_pDeviceContext{ pDeviceContext }
{
}

leap::graphics::DirectXMeshRenderer::~DirectXMeshRenderer()
{
	if (m_pInputLayout) m_pInputLayout->Release();
}

void leap::graphics::DirectXMeshRenderer::Draw()
{
	if (m_pMaterial == nullptr) return;

	// Apply the world transformation
	m_pMaterial->SetWorldMatrix(m_Transform);

	// Set primitive topology
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set input layout
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	// Set vertex buffer
	constexpr UINT stride{ sizeof(Vertex) };
	constexpr UINT offset{ 0 };
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set index buffer
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	D3DX11_TECHNIQUE_DESC techniqueDesc{};
	m_pMaterial->GetTechnique()->GetDesc(&techniqueDesc);
	for (UINT p{}; p < techniqueDesc.Passes; ++p)
	{
		m_pMaterial->GetTechnique()->GetPassByIndex(p)->Apply(0, m_pDeviceContext);
		m_pDeviceContext->DrawIndexed(m_NrIndices, 0, 0);
	}
}

leap::graphics::IMaterial* leap::graphics::DirectXMeshRenderer::GetMaterial()
{
	return m_pMaterial;
}

void leap::graphics::DirectXMeshRenderer::SetMaterial(IMaterial* pMaterial)
{
	m_pMaterial = static_cast<DirectXMaterial*>(pMaterial);

	// Reload the input layout
	if (m_pInputLayout) m_pInputLayout->Release();
	m_pInputLayout = m_pMaterial->LoadInputLayout(m_pDevice);
}

void leap::graphics::DirectXMeshRenderer::SetTransform(const glm::mat4x4& transform)
{
	m_Transform = transform;
}

void leap::graphics::DirectXMeshRenderer::LoadMesh(const std::string& filePath)
{
	m_FilePath = filePath;

	const DirectXMeshLoader::DirectXMeshDefinition& mesh{ DirectXMeshLoader::GetInstance().LoadMesh(filePath, m_pDevice) };

	m_pVertexBuffer = mesh.vertexBuffer;
	m_pIndexBuffer = mesh.indexBuffer;
	m_NrIndices = mesh.nrIndices;
}

void leap::graphics::DirectXMeshRenderer::LoadMesh(const CustomMesh& mesh)
{
	const DirectXMeshLoader::DirectXMeshDefinition& directXMesh{ DirectXMeshLoader::GetInstance().LoadMesh(mesh, m_pDevice) };

	m_pVertexBuffer = directXMesh.vertexBuffer;
	m_pIndexBuffer = directXMesh.indexBuffer;
	m_NrIndices = directXMesh.nrIndices;
}

void leap::graphics::DirectXMeshRenderer::Reload(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	LoadMesh(m_FilePath);

	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	SetMaterial(m_pMaterial);
}
