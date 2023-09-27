#pragma once

#include "Singleton.h"

#include <unordered_map>
#include <string>

struct ID3D11Buffer;
struct ID3D11Device;

namespace leap::graphics
{
	class DirectXMeshLoader final : public Singleton<DirectXMeshLoader>
	{
	public:
		struct DirectXMeshDefinition
		{
			ID3D11Buffer* vertexBuffer{};
			ID3D11Buffer* indexBuffer{};
			unsigned int nrIndices{};
		};
		
		const DirectXMeshDefinition& LoadMesh(const std::string& dataPath, ID3D11Device* pDevice);

		void Reload(ID3D11Device* pDevice);

	private:
		DirectXMeshLoader() = default;
		~DirectXMeshLoader();

		DirectXMeshDefinition CreateMesh(const std::string& dataPath, ID3D11Device* pDevice);

		std::unordered_map<std::string, DirectXMeshDefinition> m_Meshes{};

		friend Singleton<DirectXMeshLoader>;
	};
}