#pragma once

#include "../Interfaces/ITexture.h"

#include <string>
#include <vector>
#include <memory>

#include <wincodec.h>
#include <dxgiformat.h>

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace leap::graphics
{
	class DirectXEngine;
	class DirectXTexture final : public ITexture
	{
	public:
		DirectXTexture(DirectXEngine* pEngine, int width, int height);
		DirectXTexture(DirectXEngine* pEngine, const std::string& path);
		virtual ~DirectXTexture();

		DirectXTexture(const DirectXTexture& other) = delete;
		DirectXTexture(DirectXTexture&& other) = delete;
		DirectXTexture& operator=(const DirectXTexture& other) = delete;
		DirectXTexture& operator=(DirectXTexture&& other) = delete;

		virtual void SetData(const std::vector<unsigned char>& data) override;
		virtual std::vector<unsigned char> GetData() override;
		virtual glm::uvec2 GetSize() const override;
		virtual void Remove() override;

		ID3D11ShaderResourceView* GetResource() const { return m_pSRV; };

	private:
		void LoadTexture(const std::string& path);
		void LoadTexture(int width, int height);

		static DXGI_FORMAT ConvertWICToDXGI(const WICPixelFormatGUID& wicFormat);
		static WICPixelFormatGUID ConvertWICToWIC(const WICPixelFormatGUID& wicFormatGUID);

		DirectXEngine* m_pEngine{};

		ID3D11Texture2D* m_pResource{};
		ID3D11ShaderResourceView* m_pSRV{};
	};
}