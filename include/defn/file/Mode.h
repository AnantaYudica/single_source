#ifndef INTF_FILE_MODE_H_
#define INTF_FILE_MODE_H_

#include <cstdint>

namespace intf
{
namespace file
{

typedef std::uint8_t ModeValueType;

enum class Mode : ModeValueType
{
    input = 0x01,
    output = 0x02
};

} //!file

} //!intf

inline intf::file::ModeValueType operator|(const intf::file::Mode & a,
    const intf::file::Mode & b)
{
    return static_cast<intf::file::ModeValueType>(a) |
        static_cast<intf::file::ModeValueType>(b);
}

#endif //!INTF_FILE_MODE_H_
