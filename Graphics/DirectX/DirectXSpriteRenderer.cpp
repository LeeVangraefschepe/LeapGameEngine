#include "DirectXSpriteRenderer.h"
#include "DirectXShaderReader.h"

#include "../Data/Sprite.h"
#include "DirectXMaterial.h"
#include "DirectXEngine.h"
#include "../Shaders/Sprites.h"

#include <algorithm>

#include "Debug.h"

void leap::graphics::DirectXSpriteRenderer::Create(DirectXEngine* pEngine, const glm::vec2& screenSize)
{
	m_pDeviceContext = pEngine->GetContext();
	m_pDevice = pEngine->GetDevice();

	const auto shaderData{ DirectXShaderReader::GetShaderData(shaders::Sprites::GetShader()) };
	m_pMaterial = std::make_unique<DirectXMaterial>(pEngine, shaderData.path, shaderData.vertexDataFunction);

	const float scaleX = 2.0f / float(screenSize.x);
	const float scaleY = 2.0f / float(screenSize.y);
	const glm::mat4x4 transform = glm::mat4x4
	{
		scaleX, 0,       0,   0,
		0,      scaleY,  0,   0,
		0,      0,       1,   0,
		0,      0,       0,   1
	};
	m_pMaterial->SetMat4x4("gTransform", transform);
	m_pMaterial->SetFloat2("gScreenSize", screenSize);
}

void leap::graphics::DirectXSpriteRenderer::AddSprite(Sprite* pSprite)
{
	m_pSprites.push_back(pSprite);
}

void leap::graphics::DirectXSpriteRenderer::RemoveSprite(Sprite* pSprite)
{
	m_pSprites.erase(std::remove(begin(m_pSprites), end(m_pSprites), pSprite));
}

void leap::graphics::DirectXSpriteRenderer::Draw()
{
	// Set primitive topology
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Set input layout
	m_pDeviceContext->IASetInputLayout(m_pMaterial->GetInputLayout());

	// Sort the sprites on z depth
	std::stable_sort(begin(m_pSprites), end(m_pSprites), [](Sprite* pSprite1, Sprite* pSprite2)
		{
			return pSprite1->vertex.position.z < pSprite2->vertex.position.z;
		});

	// Draw sprites
	for (Sprite* pSprite : m_pSprites)
	{
		if (pSprite->pTexture == nullptr) continue;

		pSprite->OnDraw();
		DrawSprite(pSprite);
	}
}

void leap::graphics::DirectXSpriteRenderer::DrawSprite(Sprite* pSprite) const
{
	// Set sprite texture
	m_pMaterial->SetTexture("gTexture", pSprite->pTexture);

	// Create vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(SpriteVertex);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	// Cache the depth value of this sprite
	const float depth{ pSprite->vertex.position.z };
	// Reset the z position to 0, to make sure the sprite is not rendered behind the near plane
	pSprite->vertex.position.z = 0.0f;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = &pSprite->vertex;

	ID3D11Buffer* pVertexBuffer{};
	HRESULT result{ m_pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer) };
	if (FAILED(result)) Debug::LogError("DirectXEngine Error: Failed to created a vertex buffer for sprites");

	// Set vertex buffer
	constexpr UINT stride{ sizeof(SpriteVertex) };
	constexpr UINT offset{ 0 };
	m_pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Draw
	D3DX11_TECHNIQUE_DESC techniqueDesc{};
	m_pMaterial->GetTechnique()->GetDesc(&techniqueDesc);
	for (UINT p{}; p < techniqueDesc.Passes; ++p)
	{
		m_pMaterial->GetTechnique()->GetPassByIndex(p)->Apply(0, m_pDeviceContext);
		m_pDeviceContext->Draw(1, 0);
	}

	// Reset the z position to the previous depth value
	pSprite->vertex.position.z = depth;

	// Release vertex buffer
	pVertexBuffer->Release();
}
