#include "DirectXMaterial.h"

#include <d3d11.h>
#include <d3dx11effect.h>
#include <d3dcompiler.h>

#include <sstream>
#include <iostream>

glm::mat4x4 leap::graphics::DirectXMaterial::m_ViewProjMatrix{};

leap::graphics::DirectXMaterial::DirectXMaterial(ID3D11Device* pDevice, const std::string& assetFile)
{
	m_pEffect = LoadEffect(pDevice, assetFile);

	// Save the technique of the effect as a member variable
	m_pTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");
	if (!m_pTechnique->IsValid()) return;

	// Save the worldviewprojection variable of the effect as a member variable
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pMatWorldViewProjVariable->IsValid()) return;
}

leap::graphics::DirectXMaterial::~DirectXMaterial()
{
	if (m_pEffect) m_pEffect->Release();
}

ID3D11InputLayout* leap::graphics::DirectXMaterial::LoadInputLayout(ID3D11Device* pDevice) const
{
	// Create vertex layout
	static constexpr unsigned int numElements{ 1 };
	D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

	vertexDesc[0].SemanticName = "POSITION";
	vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[0].AlignedByteOffset = 0;
	vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	//vertexDesc[2].SemanticName = "NORMAL";
	//vertexDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//vertexDesc[2].AlignedByteOffset = 12;
	//vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	//vertexDesc[1].SemanticName = "TANGENT";
	//vertexDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//vertexDesc[1].AlignedByteOffset = 24;
	//vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	//vertexDesc[3].SemanticName = "TEXCOORD";
	//vertexDesc[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	//vertexDesc[3].AlignedByteOffset = 36;
	//vertexDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	// Create input layout
	D3DX11_PASS_DESC passDesc{};
	m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

	ID3D11InputLayout* pInputLayout;

	const HRESULT result{ pDevice->CreateInputLayout
		(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&pInputLayout
		) };
	if (FAILED(result)) return nullptr;

	return pInputLayout;
}

ID3DX11EffectTechnique* leap::graphics::DirectXMaterial::GetTechnique() const
{
	return m_pTechnique;
}

void leap::graphics::DirectXMaterial::SetViewProjectionMatrix(const glm::mat4x4& viewProjMatrix)
{
	m_ViewProjMatrix = viewProjMatrix;
}

void leap::graphics::DirectXMaterial::SetWorldMatrix(const glm::mat4x4& worldMatrix)
{
	const glm::mat4x4 wvpMatrix{ m_ViewProjMatrix * worldMatrix };
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<const float*>(&wvpMatrix));
}

ID3DX11Effect* leap::graphics::DirectXMaterial::LoadEffect(ID3D11Device* pDevice, const std::string& assetFile)
{
	HRESULT result;
	ID3D10Blob* pErrorBlob{ nullptr };
	ID3DX11Effect* pEffect{};

	DWORD shaderFlags{ 0 };

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	std::wstringstream wAssetFile{};
	wAssetFile << assetFile.c_str();

	// Load the effect form the file
	result = D3DX11CompileEffectFromFile
	(
		wAssetFile.str().c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		0,
		pDevice,
		&pEffect,
		&pErrorBlob
	);

	// If loading the effect failed, print an error message
	if (FAILED(result))
	{
		if (pErrorBlob != nullptr)
		{
			const char* pErrors{ static_cast<char*>(pErrorBlob->GetBufferPointer()) };

			std::stringstream ss;
			for (unsigned int i{}; i < pErrorBlob->GetBufferSize(); ++i)
			{
				ss << pErrors[i];
			}

			OutputDebugString(ss.str().c_str());
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			std::cout << ss.str() << "\n";
		}
		else
		{
			std::stringstream ss;
			ss << "EffectLoader: Failed to CreateEffectFromFile!\nPath: " << assetFile;
			std::cout << ss.str() << "\n";
			return nullptr;
		}
	}

	return pEffect;
}
