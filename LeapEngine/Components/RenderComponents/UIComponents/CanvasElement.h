#pragma once

#include <vec2.hpp>

#include "../../Component.h"

namespace leap
{
	class CanvasElement final : public Component
	{
	public:
		CanvasElement() = default;
		virtual ~CanvasElement() = default;

		CanvasElement(const CanvasElement& other) = delete;
		CanvasElement(CanvasElement&& other) = delete;
		CanvasElement& operator=(const CanvasElement& other) = delete;
		CanvasElement& operator=(CanvasElement&& other) = delete;

		/// <summary>
		/// Set the reference size of this canvas element
		/// </summary>
		void SetSize(const glm::vec2& size);
		/// <summary>
		/// Set the reference size of this canvas element
		/// </summary>
		void SetSize(float x, float y);
		/// <summary>
		/// Set the reference size of this canvas element
		/// </summary>
		void SetSize(float size);

		/// <summary>
		/// Set the reference size of this canvas element to the size of the bound sprite
		/// </summary>
		void SetNativeSize();

	protected:
		virtual void Awake() override;

	private:
		enum class DirtyFlag
		{
			None		= 0,
			NativeSize	= 1,
			Resize		= 2
		};

		unsigned char m_DirtyFlag{};

		void InternalSetNativeSize();

		bool IsDirty(DirtyFlag dirtyFlag) const;
		void SetDirty(DirtyFlag dirtyFlag, bool enabled);
	};
}