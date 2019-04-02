#ifndef INTF_FILE_OUTPUT_H_
#define INTF_FILE_OUTPUT_H_

#include "../../defn/file/Mode.h"
#include "../../defn/file/Offset.h"
#include "../../defn/file/Position.h"
#include "../../defn/file/Size.h"
#include "../../defn/file/Way.h"

namespace intf
{
namespace file
{

class Output
{
public:
    typedef defn::file::OffsetType OffsetType;
    typedef defn::file::PositionType PositionType;
    typedef defn::file::SizeType SizeType;
    typedef defn::file::Mode ModeType;
    typedef defn::file::ModeValueType ModeValueType;
    typedef defn::file::Way WayType;
public:
    Output() = default;
public:
    virtual ~Output() = default;
public:
    virtual bool IsOpen() = 0;
private:
    virtual PositionType SeekPosition(const PositionType & pos,
        const ModeValueType & mode) = 0; 
    virtual PositionType SeekOffset(const OffsetType & off,
        const WayType & way, const ModeValueType & mode) = 0;
public:
    PositionType SeekPosition(const PositionType & pos); 
    PositionType SeekOffset(const OffsetType & off,
        const WayType & way);
public:
    virtual SizeType Put(const char * buffer, const SizeType & size) = 0;
    virtual SizeType Put(const char & ch) = 0;
public:
    virtual SizeType CurrentPut(const char * buffer, 
        const SizeType & size) = 0;
    virtual SizeType CurrentPut(const char & ch) = 0;
};

} //!file

} //!intf

#endif //!INTF_FILE_OUTPUT_H_
