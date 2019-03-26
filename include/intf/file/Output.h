#ifndef INTF_FILE_OUTPUT_H_
#define INTF_FILE_OUTPUT_H_

#include "../defn/file/Offset.h"
#include "../defn/file/Position.h"
#include "../defn/file/Size.h"

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
public:
    Output() = default;
public:
    virtual ~Output() = default;
public:
    virtual bool IsOpen() = 0;
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
