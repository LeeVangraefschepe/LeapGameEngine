#pragma once

#include "../ShaderDelete.h"

#include <string>
#include <memory>

#include <vec3.hpp>

namespace leap::graphics
{
	class Camera;
	class IMeshRenderer;
	class IMaterial;
	struct Shader;

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void Initialize() = 0;
		virtual void Draw() = 0;
		virtual void SetActiveCamera(Camera*) = 0;
		virtual Camera* GetCamera() const = 0;
		virtual IMeshRenderer* CreateMeshRenderer() = 0;
		virtual void RemoveMeshRenderer(IMeshRenderer* pMeshRenderer) = 0;
		virtual IMaterial* CreateMaterial(std::unique_ptr<Shader, ShaderDelete> pShader) = 0;
		virtual void SetDirectionLight(const glm::vec3& direction) = 0;
	};

	class DefaultRenderer final : public IRenderer
	{
	public:
		virtual ~DefaultRenderer() = default;
		virtual void Initialize() override {}
		virtual void Draw() override {}
		virtual void SetActiveCamera(Camera*) override {}
		virtual Camera* GetCamera() const override { return nullptr; }
		virtual IMeshRenderer* CreateMeshRenderer() override  { return nullptr; }
		virtual void RemoveMeshRenderer(IMeshRenderer*) override {}
		virtual IMaterial* CreateMaterial(std::unique_ptr<Shader, ShaderDelete>) override { return nullptr; };
		virtual void SetDirectionLight(const glm::vec3&) override {};
	};
}