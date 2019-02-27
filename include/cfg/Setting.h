#ifndef CFG_SETTING_H_
#define CFG_SETTING_H_

#include <cstddef>

namespace cfg
{

struct Setting
{
    bool EnableThread;
    std::size_t ThreadSize;
    std::size_t StringBufferSize;
    std::size_t PathnameManagementQueueSize;
};

} //!cfg

#endif //!CFG_SETTING_H_