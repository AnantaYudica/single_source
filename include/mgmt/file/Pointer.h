#ifndef MGMT_FILE_POINTER_H_
#define MGMT_FILE_POINTER_H_

#include "../../defn/mgmt/pathname/Key.h"
#include "../../defn/file/Mode.h"
#include "../../defn/file/Offset.h"
#include "../../defn/file/Position.h"
#include "../../defn/file/Size.h"
#include "../../defn/file/Way.h"
#include "../../intf/file/Input.h"
#include "../../intf/file/Output.h"
#include "../../intf/File.h"

#include <mutex>
#include <atomic>

namespace mgmt
{
namespace file
{

class Pointer :
    public intf::file::Input,
    public intf::file::Output
{
public:
    typedef defn::mgmt::pathname::KeyValue KeyPathnameType;
    typedef defn::file::OffsetType OffsetType;
    typedef defn::file::PositionType PositionType;
    typedef defn::file::SizeType SizeType;
    typedef defn::file::Way WayType;
    typedef defn::file::Mode ModeType;
    typedef defn::file::ModeValueType ModeValueType;
private:
    bool m_lock;
    KeyPathnameType m_key;
    std::mutex * m_mutex;
    intf::File * m_file;
    std::atomic_size_t * m_count;
public:
    Pointer();
    Pointer(const KeyPathnameType & key, intf::File * file, std::mutex * lock,
        std::atomic_size_t * count);
    ~Pointer();
public:
    Pointer(const Pointer & cpy);
    Pointer(Pointer && mov);
public:
    Pointer & operator=(const Pointer & cpy);
    Pointer & operator=(Pointer && mov);
private:
    void Reset();
public:
    KeyPathnameType KeyPathname() const;
public:
    bool IsOpen() const;
public:
    PositionType SeekPosition(const PositionType & pos,
        const ModeValueType & mode = defn::file::mode::DefaultValue); 
    PositionType SeekOffset(const OffsetType & off,
        const WayType & way, const ModeValueType & mode =
        defn::file::mode::DefaultValue);
public:
    SizeType Put(const char * buffer, const SizeType & size);
    SizeType Put(const char & ch);
public:
    SizeType CurrentPut(const char * buffer, const SizeType & size);
    SizeType CurrentPut(const char & ch);
public:
    SizeType Get(char * buffer, const SizeType & size);
    int Get();
public:
    SizeType CurrentGet(char * buffer, const SizeType & size);
    int CurrentGet();
public:
    void Lock();
public:
    bool Trylock();
public:
    void Unlock();
public:
    operator bool() const;
public:
    bool operator==(const Pointer & other) const;
    bool operator!=(const Pointer & other) const;

};

} //!file

} //!mgmt

#endif //!MGMT_FILE_POINTER_H_
