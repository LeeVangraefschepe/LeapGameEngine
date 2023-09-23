#pragma once

namespace leap::graphics
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void Initialize() = 0;
		virtual void Draw() = 0;
	};

	class DefaultRenderer final : public IRenderer
	{
	public:
		virtual ~DefaultRenderer() = default;

		virtual void Initialize() override {}
		virtual void Draw() override {}
	};
}