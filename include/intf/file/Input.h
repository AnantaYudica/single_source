#ifndef INTF_FILE_INPUT_H_
#define INTF_FILE_INPUT_H_

#include "../defn/file/Mode.h"
#include "../defn/file/Offset.h"
#include "../defn/file/Position.h"
#include "../defn/file/Size.h"
#include "../defn/file/Way.h"

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
    typedef defn::file::Way WayType;
    typedef defn::file::Mode ModeType;
    typedef defn::file::ModeValueType ModeValueType;
public:
    Input() = default;
public:
    virtual ~Input() = default;
public:
    virtual bool IsOpen() = 0;
private:
    virtual PositionType SeekPosition(const PositionType & pos,
        const ModeValueType & mode = defn::file::mode::DefaultValue) = 0; 
    virtual PositionType SeekOffset(const OffsetType & off,
        const WayType & way, const ModeValueType & mode = 
        defn::file::mode::DefaultValue) = 0;
public:
    PositionType SeekPosition(const PositionType & pos); 
    PositionType SeekOffset(const OffsetType & off,
        const WayType & way);
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
