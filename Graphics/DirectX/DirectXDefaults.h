#pragma once

#include <Singleton.h>

#include <memory>

struct ID3D11Device;
struct ID3D11Buffer;

namespace leap::graphics
{
	class DirectXMaterial;

	class DirectXDefaults final : public Singleton<DirectXDefaults>
	{
	public:
		virtual ~DirectXDefaults();

		DirectXMaterial* GetMaterialNotFound(ID3D11Device* pDevice);
		DirectXMaterial* GetMaterialError(ID3D11Device* pDevice);
		static void GetMeshError(ID3D11Device* pDevice, ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexBuffer, unsigned int& nrIndices);

		void Reload(ID3D11Device* pDevice) const;
	private:
		friend Singleton;

		DirectXDefaults() = default;

		std::unique_ptr<DirectXMaterial> m_pNotFoundMaterial{};
		std::unique_ptr<DirectXMaterial> m_pNoMeshMaterial{};
	};
}