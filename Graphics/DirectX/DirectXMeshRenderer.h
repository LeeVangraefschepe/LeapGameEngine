#pragma once

#include "../Interfaces/IMeshRenderer.h"

#include "../Data/Vertex.h"

#include <vector>
#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11Buffer;

namespace leap::graphics
{
	class IMaterial;
	class DirectXMaterial;

	class DirectXMeshRenderer final : public IMeshRenderer
	{
	public:
		DirectXMeshRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
		virtual ~DirectXMeshRenderer();

		DirectXMeshRenderer(const DirectXMeshRenderer& other) = delete;
		DirectXMeshRenderer(DirectXMeshRenderer&& other) = delete;
		DirectXMeshRenderer& operator=(const DirectXMeshRenderer& other) = delete;
		DirectXMeshRenderer& operator=(DirectXMeshRenderer&& other) = delete;

		virtual void Draw() override;
		virtual IMaterial* GetMaterial() override;
		virtual void SetMaterial(IMaterial* pMaterial) override;

	private:
		DirectXMaterial* m_pMaterial{};

		ID3D11Device* m_pDevice{};
		ID3D11DeviceContext* m_pDeviceContext{};

		std::vector<Vertex> m_Vertices{};
		std::vector<unsigned int> m_Indices{};

		ID3D11InputLayout* m_pInputLayout{};
		ID3D11Buffer* m_pVertexBuffer{};
		ID3D11Buffer* m_pIndexBuffer{};
	};
}