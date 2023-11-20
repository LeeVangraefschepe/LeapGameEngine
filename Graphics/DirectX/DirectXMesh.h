#pragma once

#include "../Interfaces/IMesh.h"

#include <string>

struct ID3D11Buffer;

namespace leap::graphics
{
	class DirectXEngine;
	class DirectXMeshRenderer;

	class DirectXMesh final : public IMesh
	{
	public:
		DirectXMesh(DirectXEngine* pEngine);
		DirectXMesh(DirectXEngine* pEngine, const std::string& filePath);

		virtual void ReloadMesh(const CustomMesh& mesh) override;
		virtual void Remove() override;

		virtual ~DirectXMesh() = default;

	private:
		friend DirectXMeshRenderer;

		unsigned int m_VertexSize{};
		ID3D11Buffer* m_pVertexBuffer{};
		unsigned int m_NrIndices{};
		ID3D11Buffer* m_pIndexBuffer{};

		DirectXEngine* m_pEngine{};
	};
}