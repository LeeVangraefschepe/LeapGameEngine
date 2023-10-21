#pragma once

namespace leap
{
	class ILogger
	{
	public:
		ILogger() = default;
		virtual ~ILogger() = default;
		virtual void SetEnabled(bool enabled) = 0;

		ILogger(const ILogger& other) = delete;
		ILogger(ILogger&& other) = delete;
		ILogger& operator=(const ILogger& other) = delete;
		ILogger& operator=(ILogger&& other) = delete;
	protected:
		virtual void OnUpdate(){};
		virtual void OnGUI(){};
	private:
		friend class GameContext;
	};
}