#include "DirectXMeshRenderer.h"
#include <d3d11.h>
#include <d3dx11effect.h>

#include "DirectXMaterial.h"

leap::graphics::DirectXMeshRenderer::DirectXMeshRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice{ pDevice }
	, m_pDeviceContext{ pDeviceContext }
{
	// Create vertices
	m_Vertices.push_back(Vertex{ { -1.5f, -1.0f, 0.0f } });
	m_Vertices.push_back(Vertex{ { 0.0f, 1.0f, 0.0f } });
	m_Vertices.push_back(Vertex{ { 1.5f, -1.0f, 0.0f } });

	// Create indices
	m_Indices.push_back(0);
	m_Indices.push_back(1);
	m_Indices.push_back(2);

	// Create vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex) * static_cast<unsigned int>(m_Vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = m_Vertices.data();

	HRESULT result{ pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer) };
	if (FAILED(result)) return;

	// Create index buffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(unsigned int) * static_cast<unsigned int>(m_Indices.size());
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_Indices.data();

	result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(result)) return;
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
