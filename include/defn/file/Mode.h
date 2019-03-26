#ifndef DEFN_FILE_MODE_H_
#define DEFN_FILE_MODE_H_

#include <cstdint>

namespace defn
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

} //!defn

inline defn::file::ModeValueType operator|(const defn::file::Mode & a,
    const defn::file::Mode & b)
{
    return static_cast<defn::file::ModeValueType>(a) |
        static_cast<defn::file::ModeValueType>(b);
}

#endif //!DEFN_FILE_MODE_H_
