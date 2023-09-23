#pragma once

#include "Components/Component.h"

namespace unag
{
	class Transformator final : public leap::Component
	{
	public:
		Transformator() = default;
		~Transformator() = default;

		Transformator(const Transformator& other) = delete;
		Transformator(Transformator&& other) = delete;
		Transformator& operator=(const Transformator& other) = delete;
		Transformator& operator=(Transformator&& other) = delete;

	protected:
		virtual void Update() override;

	private:
	};
}