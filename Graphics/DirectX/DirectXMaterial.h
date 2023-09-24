#pragma once

#include "../Interfaces/IMaterial.h"

#include <string>
#include <vector>
#include <functional>

#include <d3dx11effect.h>

struct ID3D11InputLayout;
struct ID3D11Device;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;

namespace leap::graphics
{
	class DirectXMaterial final : public IMaterial
	{
	public:
		DirectXMaterial(ID3D11Device* pDevice, const std::string& assetFile);
		DirectXMaterial(ID3D11Device* pDevice, const std::string& assetFile, std::function<std::vector<D3D11_INPUT_ELEMENT_DESC>()> vertexDataFunction);
		virtual ~DirectXMaterial();

		DirectXMaterial(const DirectXMaterial& other) = delete;
		DirectXMaterial(DirectXMaterial&& other) = delete;
		DirectXMaterial& operator=(const DirectXMaterial& other) = delete;
		DirectXMaterial& operator=(DirectXMaterial&& other) = delete;

		ID3D11InputLayout* LoadInputLayout(ID3D11Device* pDevice) const;
		ID3DX11EffectTechnique* GetTechnique() const;

		static void SetViewProjectionMatrix(const glm::mat4x4& viewProjMatrix);
		void SetWorldMatrix(const glm::mat4x4& worldMatrix);

	private:
		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::string& assetFile);

		static glm::mat4x4 m_ViewProjMatrix;

		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};

		std::function<std::vector<D3D11_INPUT_ELEMENT_DESC>()> m_VertexDataFunction{};
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};
	};
}