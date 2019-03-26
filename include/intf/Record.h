#ifndef INTF_RECORD_H_
#define INTF_RECORD_H_

#include "file/Input.h"
#include "file/Output.h"

#include "../defn/file/Offset.h"
#include "../defn/file/Position.h"
#include "../defn/file/Size.h"

namespace intf
{

class Record
{
public:
    typedef defn::file::OffsetType OffsetType;
    typedef defn::file::PositionType PositionType;
    typedef defn::file::SizeType SizeType;
public:
    typedef file::Input InputType;
    typedef file::Output OutputType;
public:
    Record() = default;
public:
    virtual ~Record() = default;
public:
    virtual SizeType Put(OutputType & out) const = 0;
public:
    virtual SizeType Get(InputType & in) = 0;
};

} //!intf

#endif //!INTF_RECORD_H_
