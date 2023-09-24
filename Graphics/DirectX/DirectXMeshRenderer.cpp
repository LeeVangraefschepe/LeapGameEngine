#include "DirectXMeshRenderer.h"
#include <d3d11.h>
#include <d3dx11effect.h>

#include "DirectXMaterial.h"

#include "../Utils.h"

leap::graphics::DirectXMeshRenderer::DirectXMeshRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice{ pDevice }
	, m_pDeviceContext{ pDeviceContext }
{
}

leap::graphics::DirectXMeshRenderer::~DirectXMeshRenderer()
{
	if (m_pInputLayout) m_pInputLayout->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pIndexBuffer) m_pIndexBuffer->Release();
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
		m_pDeviceContext->DrawIndexed(static_cast<uint32_t>(m_Indices.size()), 0, 0);
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
	Utils::ParseObj(filePath, m_Vertices, m_Indices);

	// Create vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex) * static_cast<unsigned int>(m_Vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = m_Vertices.data();

	HRESULT result{ m_pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer) };
	if (FAILED(result)) return;

	// Create index buffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(unsigned int) * static_cast<unsigned int>(m_Indices.size());
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_Indices.data();

	result = m_pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(result)) return;
}
