#pragma once

#include <string>

namespace leap::graphics
{
	class Camera;
	class IMeshRenderer;
	class IMaterial;

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
		virtual IMaterial* CreateMaterial(const std::string& dataPath) = 0;
	};

	class DefaultRenderer final : public IRenderer
	{
	public:
		virtual ~DefaultRenderer() = default;
		virtual void Initialize() override {}
		virtual void Draw() override {}
		virtual void SetActiveCamera(Camera*) override {}
		virtual Camera* GetCamera() const override { return nullptr; }
		virtual IMeshRenderer* CreateMeshRenderer() { return nullptr; }
		virtual void RemoveMeshRenderer(IMeshRenderer*) {}
		virtual IMaterial* CreateMaterial(const std::string&) { return nullptr; }
	};
}