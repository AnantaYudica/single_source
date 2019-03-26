#ifndef DEFN_REC_SIZE_H_
#define DEFN_REC_SIZE_H_

#include <cstdint>

namespace defn
{
namespace rec
{

template<typename TRecord>
struct Size
{
    typedef std::size_t ValueType;
    static constexpr ValueType Value = sizeof(TRecord);
};

} //!rec

} //!defn

#endif //!DEFN_REC_SIZE_H_
