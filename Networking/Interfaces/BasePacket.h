#pragma once
#include <vector>

namespace leap::networking
{
    class BasePacket
    {
    public:
        virtual char* GetData(){ return &*m_data.begin(); }
        virtual void SetData(std::vector<char>& data) { m_data = std::move(data); }
        virtual int Length() const { return static_cast<int>(m_data.size()); }

        virtual ~BasePacket() = default;
    protected:
        std::vector<char> m_data{};
    };
}