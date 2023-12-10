#include "FileLogger.h"
#include <fstream>
#include <sstream>

leap::FileLogger::FileLogger()
{
    SetEnabled(true);
}

leap::FileLogger::~FileLogger()
{
    SetEnabled(false);
}

void leap::FileLogger::SetEnabled(bool enable)
{
    if (m_Enabled == enable) return;

    if (enable)
    {
        StartFile();
        Debug::OnEvent.AddListener(this, &FileLogger::OnDebug);
    }
    else
    {
        CloseFile();
        Debug::OnEvent.RemoveListener(this, &FileLogger::OnDebug);
    }
    m_Enabled = enable;
}

void leap::FileLogger::SetPath(const std::string& path)
{
	const std::string oldPath = m_Path;
    m_Path = path;

    if (m_File.is_open())
    {
        CloseFile();
        StartFile();

        std::stringstream ss{};
        ss << "Already logged to " << oldPath;
        Debug::LogWarning(ss.str());
    }
}

void leap::FileLogger::StartFile()
{
    m_File = std::fstream{ m_Path, std::fstream::out | std::fstream::trunc };
    if (m_File.is_open() == false)
    {
        SetEnabled(false);
        std::stringstream ss{};
        ss << "Failed to open file: " << m_Path;
        Debug::LogWarning(ss.str());
        if (m_File.fail()) Debug::LogWarning("Failed open file: General I/O failure");
        if (m_File.bad()) Debug::LogWarning("Failed open file: Bad file stream or files system error");
        if (m_File.eof()) Debug::LogWarning("Failed open file: The end of the file was reached during opening");
    }
}

void leap::FileLogger::CloseFile()
{
    m_File.close();
}

void leap::FileLogger::OnDebug(const Debug::LogInfo& data)
{
    if (!m_File.is_open())
    {
        SetEnabled(false);
        Debug::LogWarning("File logger can not log. File is not open.");
	    return;
    }

    switch (data.Type)
    {
    case Debug::Type::Message:
        m_File << "[Message]\n";
        break;
    case Debug::Type::Warning:
        m_File << "[Warning]\n";
        break;
    case Debug::Type::Error:
        m_File << "[Error]\n";
        break;
    }
    
    m_File << data.Time << ' ' << data.Message << '\n';
    m_File << "** " << data.Location.file_name() << " line " << data.Location.line() << ':' << data.Location.column() << "\n\n";
}
