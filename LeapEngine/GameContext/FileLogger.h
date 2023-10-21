#pragma once
#include "Observer.h"
#include "Debug.h"
#include <string>
#include <fstream>

namespace leap
{
	class FileLogger final : Observer<Debug::LogInfo>
	{
	public:
		FileLogger() = default;
		virtual ~FileLogger();
		FileLogger(const FileLogger& other) = delete;
		FileLogger(FileLogger&& other) = delete;
		FileLogger& operator=(const FileLogger& other) = delete;
		FileLogger& operator=(FileLogger&& other) = delete;

		void SetEnabled(bool enable);
		void SetPath(const std::string& path);
	private:
		friend class GameContext;

		void StartFile();
		void CloseFile();

		void Notify(const Debug::LogInfo& data) override;
		bool m_Enabled{ false };
		std::string m_Path{ "log.txt" };
		std::fstream m_File{};
	};
}