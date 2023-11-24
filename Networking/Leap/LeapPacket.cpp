#include "LeapPacket.h"

void leap::networking::LeapPacket::SetData(std::vector<char>& data)
{
    m_Data = std::move(data);
}

char* leap::networking::LeapPacket::GetData()
{
    return m_Data.data();
}

void leap::networking::LeapPacket::ReadString(std::string& string)
{
    // Find size
    const auto size = Read<unsigned int>();

    // Set iterator ready
    auto it = m_Data.begin();
    advance(it, size);

    // Create ss with binary value
    std::stringstream ss(accumulate(m_Data.begin(), it, std::string("")), std::ios_base::in | std::ios_base::binary);

    // Read value
    std::vector<char> buffer(size);
    ss.read(buffer.data(), size);

    // Return string
    string = { buffer.data(), size };
}

void leap::networking::LeapPacket::WriteString(const std::string& string)
{
    // Get size & write it
    const auto size = static_cast<unsigned>(string.size());
    Write<unsigned int>(size);

    // Write string data binary
    std::stringstream ss{ std::ios::out | std::ios::binary };
    ss.write(string.c_str(), size);

    // Save binary string data to m_Data
    for (const auto data{ ss.str() }; char byte : data)
    {
        m_Data.push_back(byte);
    }
}

int leap::networking::LeapPacket::ReadHeaderId()
{
	return Read<int>();
}