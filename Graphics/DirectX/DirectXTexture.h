#pragma once

#include "../Interfaces/ITexture.h"

#include <string>
#include <memory>

#include <wincodec.h>
#include <dxgiformat.h>

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

		virtual glm::vec4 GetPixel(int x, int y) const override;
		virtual glm::ivec2 GetSize() const override;

		ID3D11ShaderResourceView* GetResource() const { return m_pSRV; };

		void Reload(ID3D11Device* pDevice, const std::string& path);

	private:
		void LoadTexture(ID3D11Device* pDevice, const std::string& path);

		static DXGI_FORMAT ConvertWICToDXGI(const WICPixelFormatGUID& wicFormat);
		static WICPixelFormatGUID ConvertWICToWIC(const WICPixelFormatGUID& wicFormatGUID);

		ID3D11Texture2D* m_pResource{};
		ID3D11ShaderResourceView* m_pSRV{};
	};
}