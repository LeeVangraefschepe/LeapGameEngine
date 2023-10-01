#pragma once

#include "../ShaderDelete.h"
#include "../Data/RenderData.h"

#include <string>
#include <memory>

#include <vec3.hpp>
#include <mat3x3.hpp>

namespace leap::graphics
{
	class Camera;
	class IMeshRenderer;
	class IMaterial;
	class ITexture;
	struct Shader;

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		// Internal functions
		virtual void Initialize() = 0;
		virtual void Draw() = 0;

		// Renderer settings
		virtual void SetAntiAliasing(AntiAliasing antiAliasing) = 0;
		virtual void SetWindowSize(const glm::ivec2& size) = 0;
		virtual void SetShadowMapData(unsigned int shadowMapWidth, unsigned int shadowMapHeight, float orthoSize, float nearPlane, float farPlane) = 0;

		// Graphics space objects
		virtual void SetActiveCamera(Camera* pCamera) = 0;
		virtual Camera* GetCamera() const = 0;
		virtual void SetDirectionLight(const glm::mat3x3& direction) = 0;

		// Meshes
		virtual IMeshRenderer* CreateMeshRenderer() = 0;
		virtual void RemoveMeshRenderer(IMeshRenderer* pMeshRenderer) = 0;

		// Materials & Textures
		virtual IMaterial* CreateMaterial(std::unique_ptr<Shader, ShaderDelete> pShader, const std::string& name) = 0;
		virtual ITexture* CreateTexture(const std::string& path) = 0;
	};

	class DefaultRenderer final : public IRenderer
	{
	public:
		virtual ~DefaultRenderer() = default;

		// Internal functions
		virtual void Initialize() override {}
		virtual void Draw() override {}

		// Renderer settings
		virtual void SetAntiAliasing(AntiAliasing) override {}
		virtual void SetWindowSize(const glm::ivec2&) {}
		virtual void SetShadowMapData(unsigned int, unsigned int, float, float, float) override {}

		// Graphics space objects
		virtual void SetActiveCamera(Camera*) override {}
		virtual Camera* GetCamera() const override { return nullptr; }
		virtual void SetDirectionLight(const glm::mat3x3&) override {};

		// Meshes
		virtual IMeshRenderer* CreateMeshRenderer() override  { return nullptr; }
		virtual void RemoveMeshRenderer(IMeshRenderer*) override {}

		// Materials & Textures
		virtual IMaterial* CreateMaterial(std::unique_ptr<Shader, ShaderDelete>, const std::string&) override { return nullptr; };
		virtual ITexture* CreateTexture(const std::string&) override { return nullptr; };
	};
}