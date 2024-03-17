#pragma once

#include <Singleton.h>

#include <memory>

struct ID3D11Device;
struct ID3D11Buffer;

namespace leap::graphics
{
	class DirectXMesh;
	class DirectXEngine;
	class DirectXMaterial;

	class DirectXDefaults final : public Singleton<DirectXDefaults>
	{
	public:
		virtual ~DirectXDefaults();

		DirectXMaterial* GetMaterialNotFound(DirectXEngine* pEngine);
		DirectXMaterial* GetMaterialError(DirectXEngine* pEngine);
		static DirectXMesh* GetMeshError(DirectXEngine* pEngine);
	private:
		friend Singleton;

		DirectXDefaults() = default;

		std::unique_ptr<DirectXMaterial> m_pNotFoundMaterial{};
		std::unique_ptr<DirectXMaterial> m_pNoMeshMaterial{};
	};
}