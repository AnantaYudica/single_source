#ifndef INTF_FILE_WAY_H_
#define INTF_FILE_WAY_H_

#include <cstdint>

namespace intf
{
namespace file
{

typedef std::uint8_t WayValueType;

enum class Way : WayValueType
{
    current,
    begin,
    end,
};

} //!file

} //!intf

#endif //!INTF_FILE_WAY_H_
