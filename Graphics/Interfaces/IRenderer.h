#pragma once

#include "../ShaderDelete.h"
#include "../Data/RenderData.h"

#include <string>
#include <memory>
#include <vector>

#include <vec3.hpp>
#include <mat3x3.hpp>

namespace leap::graphics
{
	class Camera;
	class IMeshRenderer;
	class IMaterial;
	class ITexture;
	class IMesh;
	struct Shader;
	struct Sprite;

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		// Internal functions
		virtual void Initialize() = 0;
		/**
		 * Calls the next frame for ImGui (full render is done in draw) 
		 */
		virtual void GuiDraw() = 0;
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
		virtual IMesh* CreateMesh(const std::string& path, bool cached) = 0;
		virtual IMesh* CreateMesh() = 0;
		virtual void RemoveMesh(IMesh* pMesh) = 0;

		// Sprites
		virtual void AddSprite(Sprite* pSprite) = 0;
		virtual void RemoveSprite(Sprite* pSprite) = 0;

		// Materials & Textures
		virtual IMaterial* CreateMaterial(std::unique_ptr<Shader, ShaderDelete> pShader, const std::string& name, bool cached) = 0;
		virtual IMaterial* CloneMaterial(const std::string& original, const std::string& clone, bool cached) = 0;
		virtual void RemoveMaterial(IMaterial* pMaterial) = 0;
		virtual ITexture* CreateTexture(const std::string& path, bool cached) = 0;
		virtual ITexture* CreateTexture(int width, int height) = 0;
		virtual void RemoveTexture(ITexture* pTexture) = 0;

		// Debug rendering
		virtual void DrawLines(const std::vector<std::pair<glm::vec3, glm::vec3>>& lines) = 0;
		virtual void DrawLine(const glm::vec3& start, const glm::vec3& end) = 0;
	};

	class DefaultRenderer final : public IRenderer
	{
	public:
		virtual ~DefaultRenderer() = default;

		// Internal functions
		virtual void Initialize() override {}
		virtual void Draw() override {}
		virtual void GuiDraw() override {}

		// Renderer settings
		virtual void SetAntiAliasing(AntiAliasing) override {}
		virtual void SetWindowSize(const glm::ivec2&) override {}
		virtual void SetShadowMapData(unsigned int, unsigned int, float, float, float) override {}

		// Graphics space objects
		virtual void SetActiveCamera(Camera*) override {}
		virtual Camera* GetCamera() const override { return nullptr; }
		virtual void SetDirectionLight(const glm::mat3x3&) override {}

		// Meshes
		virtual IMeshRenderer* CreateMeshRenderer() override  { return nullptr; }
		virtual void RemoveMeshRenderer(IMeshRenderer*) override {}
		virtual IMesh* CreateMesh(const std::string&, bool) override { return nullptr; }
		virtual IMesh* CreateMesh() override { return nullptr; }
		virtual void RemoveMesh(IMesh*) override {}

		// Sprites
		virtual void AddSprite(Sprite*) override {}
		virtual void RemoveSprite(Sprite*) override {}

		// Materials & Textures
		virtual IMaterial* CreateMaterial(std::unique_ptr<Shader, ShaderDelete>, const std::string&, bool) override { return nullptr; }
		virtual IMaterial* CloneMaterial(const std::string&, const std::string&, bool) override { return nullptr; }
		virtual void RemoveMaterial(IMaterial*) override {}
		virtual ITexture* CreateTexture(const std::string&, bool) override { return nullptr; }
		virtual ITexture* CreateTexture(int, int) override { return nullptr; }
		virtual void RemoveTexture(ITexture*) override {}

		// Debug rendering
		virtual void DrawLines(const std::vector<std::pair<glm::vec3, glm::vec3>>&) override {}
		virtual void DrawLine(const glm::vec3&, const glm::vec3&) override {}
	};
}