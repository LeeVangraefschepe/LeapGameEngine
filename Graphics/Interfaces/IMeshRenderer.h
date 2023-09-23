#pragma once

namespace leap::graphics
{
	class IMaterial;

	class IMeshRenderer 
	{
	public:
		virtual ~IMeshRenderer() = default;

		virtual void Draw() = 0;
		virtual IMaterial* GetMaterial() = 0;
		virtual void SetMaterial(IMaterial* pMaterial) = 0;
	};
}