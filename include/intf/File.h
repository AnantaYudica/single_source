#ifndef INTF_FILE_H_
#define INTF_FILE_H_

#include "file/Mode.h"
#include "file/Way.h"

#include <cstdint>
#include <string>
#include <memory>

namespace intf
{

class File
{
public:
    typedef std::shared_ptr<File> PointerType;
public:
    typedef std::uint64_t PositionType;
    typedef std::int64_t OffsetType;
    typedef std::uint64_t SizeType;
public:
    typedef file::Mode ModeType;
    typedef file::ModeValueType ModeValueType;
    typedef file::Way WayType;
public:
    File() = default;
    virtual ~File() = default;
public:
    File(const File &) = delete;
    File(File &&) = default;
public:
    File & operator=(const File &) = delete;
    virtual File & operator=(File &&) = 0;
public:
    virtual bool IsOpen() = 0;
public:
    virtual File * Open(const std::string & pathname) = 0;
    virtual File * Open(const char * pathname) = 0;
public:
    virtual void Close() = 0;
public:
    virtual PositionType SeekPosition(const PositionType & pos,
        const ModeValueType & mode = (ModeType::input | ModeType::output)) = 0; 
    virtual PositionType SeekOffset(const OffsetType & off,
        const WayType & way, const ModeValueType & mode = (ModeType::input | 
        ModeType::output)) = 0;
public:
    virtual SizeType Put(const char * buffer, const SizeType & size) = 0;
    virtual SizeType Put(const char & ch) = 0;
public:
    virtual SizeType Get(char * buffer, const SizeType & size) = 0;
    virtual int Get() = 0;
public:
    virtual int Current() = 0;
public:
    virtual bool IsEndOfFile() = 0;
};

} //!intf

#endif //!INTF_FILE_H_