#pragma once
#include "Components/Component.h"
namespace unag
{
	class InfoUI final : public leap::Component
	{
	public:
		InfoUI() = default;
		virtual ~InfoUI() = default;
		InfoUI(const InfoUI& other) = delete;
		InfoUI(InfoUI&& other) = delete;
		InfoUI& operator=(const InfoUI& other) = delete;
		InfoUI& operator=(InfoUI&& other) = delete;
	private:
		virtual void OnGUI() override;
	};
}