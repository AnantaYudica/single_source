#include "Configure.h"

using namespace std;

Configure Configure::ms_instance_cfg;

Configure & Configure::GetInstance()
{
    return ms_instance_cfg;
}

Configure::Configure()
{}

template<typename TValue>
bool Configure::Validate(const TValue & value, const TValue & min,
    const TValue & max)
{
    return min == max || (value >= min && value <= max);
}

void Configure::SetMinimum(const cfg::Setting & setting)
{
    m_minimum_setting.ThreadSize = setting.ThreadSize;
    m_minimum_setting.StringBufferSize = setting.StringBufferSize;
}

void Configure::SetMaximum(const cfg::Setting & setting)
{
    m_maximum_setting.ThreadSize = setting.ThreadSize;
    m_maximum_setting.StringBufferSize = setting.StringBufferSize;
}

void Configure::Set(const cfg::Setting & setting)
{
    m_value_setting.EnableThread = setting.EnableThread;

    if (Validate(setting.ThreadSize, m_minimum_setting.ThreadSize,
        m_maximum_setting.ThreadSize))
            m_value_setting.ThreadSize = setting.ThreadSize;
    if (Validate(setting.StringBufferSize, m_minimum_setting.StringBufferSize,
        m_maximum_setting.StringBufferSize))
            m_value_setting.StringBufferSize = setting.StringBufferSize;
    if (Validate(setting.PathnameManagementQueueSize, 
        m_minimum_setting.PathnameManagementQueueSize,
        m_maximum_setting.PathnameManagementQueueSize))
            m_value_setting.PathnameManagementQueueSize = 
                setting.PathnameManagementQueueSize;
}

bool Configure::EnableThread() const
{
    return m_value_setting.EnableThread;
}

std::size_t Configure::GetThreadSize() const
{
    return m_value_setting.ThreadSize;
}

std::size_t Configure::GetStringBufferSize() const
{
    return m_value_setting.StringBufferSize;
}

std::size_t Configure::GetPathnameManagementQueueSize() const
{
    return m_value_setting.PathnameManagementQueueSize;
}
