#pragma once

#include "../Interfaces/ITexture.h"

#include <string>
#include <vector>
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
		DirectXTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, int width, int height);
		DirectXTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& path);
		virtual ~DirectXTexture();

		DirectXTexture(const DirectXTexture& other) = delete;
		DirectXTexture(DirectXTexture&& other) = delete;
		DirectXTexture& operator=(const DirectXTexture& other) = delete;
		DirectXTexture& operator=(DirectXTexture&& other) = delete;

		virtual void SetData(void* pData, unsigned int nrBytes) override;
		virtual glm::ivec2 GetSize() const override;

		ID3D11ShaderResourceView* GetResource() const { return m_pSRV; };

		void Reload(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& path);
		void StoreData(ID3D11Device* pDevice);
		void Reload(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	private:
		std::unique_ptr<std::vector<unsigned char>> m_pData{};

		void LoadTexture(ID3D11Device* pDevice, const std::string& path);
		void LoadTexture(ID3D11Device* pDevice, int width, int height);

		static DXGI_FORMAT ConvertWICToDXGI(const WICPixelFormatGUID& wicFormat);
		static WICPixelFormatGUID ConvertWICToWIC(const WICPixelFormatGUID& wicFormatGUID);

		ID3D11DeviceContext* m_pDeviceContext{};

		ID3D11Texture2D* m_pResource{};
		ID3D11ShaderResourceView* m_pSRV{};

		
	};
}