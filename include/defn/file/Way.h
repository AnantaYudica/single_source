#ifndef DEFN_FILE_WAY_H_
#define DEFN_FILE_WAY_H_

#include <cstdint>

namespace defn
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

} //!defn

#endif //!DEFN_FILE_WAY_H_
