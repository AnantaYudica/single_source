#ifndef INTF_FILE_INPUT_H_
#define INTF_FILE_INPUT_H_

#include "../defn/file/Offset.h"
#include "../defn/file/Position.h"
#include "../defn/file/Size.h"

namespace intf
{
namespace file
{

class Input
{
public:
    typedef defn::file::OffsetType OffsetType;
    typedef defn::file::PositionType PositionType;
    typedef defn::file::SizeType SizeType;
public:
    Input() = default;
public:
    virtual ~Input() = default;
public:
    virtual bool IsOpen() = 0;
public:
    virtual SizeType Get(char * buffer, const SizeType & size) = 0;
    virtual int Get() = 0;
public:
    virtual SizeType CurrentGet(char * buffer, const SizeType & size) = 0;
    virtual int CurrentGet() = 0;
};

} //!file

} //!intf

#endif //!INTF_FILE_INPUT_H_
