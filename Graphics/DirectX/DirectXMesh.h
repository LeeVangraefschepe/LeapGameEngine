#pragma once

#include "../Interfaces/IMesh.h"

#include <string>

struct ID3D11Buffer;
struct ID3D11Device;

namespace leap::graphics
{
	class DirectXMeshRenderer;

	class DirectXMesh final : public IMesh
	{
	public:
		DirectXMesh(ID3D11Device* pDevice);
		DirectXMesh(ID3D11Device* pDevice, const std::string& filePath);

		virtual void ReloadMesh(const CustomMesh& mesh) override;

		virtual ~DirectXMesh() = default;

	private:
		friend DirectXMeshRenderer;

		unsigned int m_VertexSize{};
		ID3D11Buffer* m_pVertexBuffer{};
		unsigned int m_NrIndices{};
		ID3D11Buffer* m_pIndexBuffer{};

		ID3D11Device* m_pDevice{};
	};
}