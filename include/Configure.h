#ifndef CONFIGURE_H_
#define CONFIGURE_H_

#include <cstddef>

#include "cfg/Setting.h"

class Configure final
{
private:
    cfg::Setting m_value_setting;
    cfg::Setting m_minimum_setting;
    cfg::Setting m_maximum_setting;
private:
    static Configure ms_instance_cfg;
public:
    static Configure & GetInstance();
private:
    Configure();
public:
    Configure(const Configure &) = delete;
    Configure(Configure &&) = delete;
public:
    Configure & operator=(const Configure &) = delete;
    Configure & operator=(Configure &&) = delete;
private:
    template<typename TValue>
    bool Validate(const TValue & value, const TValue & min,
        const TValue & max);
public:
    void SetMinimum(const cfg::Setting & setting);
    void SetMaximum(const cfg::Setting & setting);
    void Set(const cfg::Setting & setting);
public:
    bool EnableThread() const;
public:
    std::size_t GetThreadSize() const;
    std::size_t GetStringBufferSize() const;
    std::size_t GetPathnameManagementQueueSize() const;
};

#endif //!CONFIGURE_H_