#include "DirectXTexture.h"

#include <d3d11.h>

//#include <WICTextureLoader.h>
#include <wincodec.h>

#include <sstream>

leap::graphics::DirectXTexture::DirectXTexture(ID3D11Device* pDevice, const std::string& path)
{
	// Load the image using WIC
	IWICImagingFactory* wicFactory{};
	HRESULT result{ CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory)) };
	if (result != S_OK) throw std::runtime_error{ "Failed to create WIC factory when creating a texture" };

	std::wstringstream wss{};
	wss << path.c_str();
	IWICBitmapDecoder* wicDecoder;
	result = wicFactory->CreateDecoderFromFilename(wss.str().c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &wicDecoder);
	if (result != S_OK) throw std::runtime_error{ "Failed to load image using the WIC factory" };

	// Get the WIC frame from the image
	IWICBitmapFrameDecode* wicFrame{};
	wicDecoder->GetFrame(0, &wicFrame);

	// Get the dimensions from the WIC frame
	UINT wicWidth{}, wicHeight{};
	wicFrame->GetSize(&wicWidth, &wicHeight);

	// Get the pixel data from the WIC frame
	UINT stride = wicWidth * 4; // Assuming 32-bit RGBA format
	UINT bufferSize = stride * wicHeight;
	BYTE* pixelData = new BYTE[bufferSize];
	wicFrame->CopyPixels(nullptr, stride, bufferSize, pixelData);

	// Create a texture from the pixel data
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = wicWidth;   // Set to the width of your image
	desc.Height = wicHeight; // Set to the height of your image
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Change the format if necessary
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = pixelData;
	initData.SysMemPitch = stride;

	result = pDevice->CreateTexture2D(&desc, &initData, &m_pResource);
	if (result != S_OK) throw std::runtime_error{ "Failed to create directX texture using the WIC frame" };

	// Create shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	pDevice->CreateShaderResourceView(m_pResource, &srvDesc, &m_pSRV);
	if (result != S_OK) throw std::runtime_error{ "Failed to create shader resource view with the given texture" };

	// Clean up resources
	delete[] pixelData;
	wicFactory->Release();
	wicDecoder->Release();
	wicFrame->Release();
}

leap::graphics::DirectXTexture::~DirectXTexture()
{
	if(m_pResource) m_pResource->Release();
	if(m_pSRV) m_pSRV->Release();
}

glm::vec4 leap::graphics::DirectXTexture::GetPixel(int /*x*/, int /*y*/)
{
	return {};
}
