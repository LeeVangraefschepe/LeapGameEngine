#pragma once

#include "../Interfaces/ITexture.h"

#include <string>
#include <memory>

struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;

namespace leap::graphics
{
	class DirectXTexture final : public ITexture
	{
	public:
		DirectXTexture(ID3D11Device* pDevice, const std::string& path);
		virtual ~DirectXTexture();

		DirectXTexture(const DirectXTexture& other) = delete;
		DirectXTexture(DirectXTexture&& other) = delete;
		DirectXTexture& operator=(const DirectXTexture& other) = delete;
		DirectXTexture& operator=(DirectXTexture&& other) = delete;

		virtual glm::vec4 GetPixel(int x, int y) override;

	private:

		ID3D11Texture2D* m_pResource{};
		ID3D11ShaderResourceView* m_pSRV{};
	};
}