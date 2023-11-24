#pragma once

#include "../Interfaces/IMaterial.h"

#include <string>
#include <vector>
#include <functional>
#include <memory>

struct ID3D11InputLayout;
struct ID3D11Device;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3D11ShaderResourceView;
struct D3D11_INPUT_ELEMENT_DESC;

namespace leap::graphics
{
	class DirectXEngine;
	class DirectXTexture;

	class DirectXMaterial final : public IMaterial
	{
	public:
		DirectXMaterial() = default;
		DirectXMaterial(DirectXEngine* pEngine, const std::string& assetFile, std::function<std::vector<D3D11_INPUT_ELEMENT_DESC>()> vertexDataFunction);
		virtual ~DirectXMaterial();

		DirectXMaterial(const DirectXMaterial& other) = delete;
		DirectXMaterial(DirectXMaterial&& other) = delete;
		DirectXMaterial& operator=(const DirectXMaterial& other) = delete;
		DirectXMaterial& operator=(DirectXMaterial&& other) = delete;

		ID3DX11EffectTechnique* GetTechnique() const;

		ID3D11InputLayout* GetInputLayout() const { return m_pInputLayout; }

		static void SetViewProjectionMatrix(const glm::mat4x4& viewProjMatrix);
		void SetWorldMatrix(const glm::mat4x4& worldMatrix);

		virtual void SetBool(const std::string& varName, bool data) override;
		virtual void SetFloat(const std::string& varName, float data) override;
		virtual void SetFloat2(const std::string& varName, const glm::vec2& data) override;
		virtual void SetFloat3(const std::string& varName, const glm::vec3& data) override;
		virtual void SetFloat4(const std::string& varName, const glm::vec4& data) override;
		virtual void SetMat3x3(const std::string& varName, const glm::mat3x3& data) override;
		virtual void SetMat4x4(const std::string& varName, const glm::mat4x4& data) override;
		virtual void SetTexture(const std::string& varName, ITexture* pTexture) override;
		void SetTexture(const std::string& varName, ID3D11ShaderResourceView* pSRV);

		virtual void Remove() override;

		std::unique_ptr<DirectXMaterial> Clone();

	private:
		ID3D11InputLayout* LoadInputLayout() const;
		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::string& assetFile);

		static glm::mat4x4 m_ViewProjMatrix;

		DirectXEngine* m_pEngine{};
		std::string m_AssetFile{};
		std::vector<ITexture*> m_pTextures{};

		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectMatrixVariable* m_pMatWorldVariable{};

		ID3D11InputLayout* m_pInputLayout{};

		std::function<std::vector<D3D11_INPUT_ELEMENT_DESC>()> m_VertexDataFunction{};
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};
	};
}