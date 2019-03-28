#ifndef DEFN_REC_STATUS_H_
#define DEFN_REC_STATUS_H_

namespace defn
{
namespace rec
{

typedef char StatusValueType;

enum class Status : StatusValueType
{
    initial = 0x01,
    good = 0x02,
    out_sync = 0x04,
    bad = 0x80,
};

} //!rec

} //!defn

#endif //!DEFN_REC_STATUS_H_
