#pragma once

#include "../Interfaces/IMaterial.h"

#include <string>

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
		virtual ~DirectXMaterial();

		DirectXMaterial(const DirectXMaterial& other) = delete;
		DirectXMaterial(DirectXMaterial&& other) = delete;
		DirectXMaterial& operator=(const DirectXMaterial& other) = delete;
		DirectXMaterial& operator=(DirectXMaterial&& other) = delete;

		ID3D11InputLayout* LoadInputLayout(ID3D11Device* pDevice) const;
		ID3DX11EffectTechnique* GetTechnique() const;

		void SetViewProjectionMatrix(const glm::mat4x4& viewProjMatrix);

	private:
		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::string& assetFile);

		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};

		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};
	};
}