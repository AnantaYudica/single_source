#ifndef INTF_FILE_H_
#define INTF_FILE_H_

#include "../defn/file/Mode.h"
#include "../defn/file/Offset.h"
#include "../defn/file/Position.h"
#include "../defn/file/Size.h"
#include "../defn/file/Way.h"
#include "file/Input.h"
#include "file/Output.h"

#include <cstdint>
#include <string>
#include <memory>

namespace intf
{

class File :
    public file::Input,
    public file::Output
{
public:
    typedef std::shared_ptr<File> PointerType;
public:
    typedef defn::file::OffsetType OffsetType;
    typedef defn::file::PositionType PositionType;
    typedef defn::file::SizeType SizeType;
public:
    typedef defn::file::Mode ModeType;
    typedef defn::file::ModeValueType ModeValueType;
    typedef defn::file::Way WayType;
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
    virtual ModeValueType Mode() const = 0;
public:
    virtual PositionType SeekPosition(const PositionType & pos,
        const ModeValueType & mode = defn::file::mode::DefaultValue) = 0; 
    virtual PositionType SeekOffset(const OffsetType & off,
        const WayType & way, const ModeValueType & mode = 
        defn::file::mode::DefaultValue) = 0;
public:
    virtual SizeType Put(const char * buffer, const SizeType & size) = 0;
    virtual SizeType Put(const char & ch) = 0;
    virtual SizeType Put(const char & ch, const SizeType & count) = 0;
public:
    virtual SizeType CurrentPut(const char * buffer, 
        const SizeType & size) = 0;
    virtual SizeType CurrentPut(const char & ch) = 0;
    virtual SizeType CurrentPut(const char & ch, const SizeType & count) = 0;
public:
    virtual SizeType Get(char * buffer, const SizeType & size) = 0;
    virtual int Get() = 0;
public:
    virtual SizeType CurrentGet(char * buffer, const SizeType & size) = 0;
    virtual int CurrentGet() = 0;
public:
    virtual bool IsEndOfFile() = 0;
};

} //!intf

#endif //!INTF_FILE_H_