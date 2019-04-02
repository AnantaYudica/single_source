#ifndef PATHNAME_MGMT_REC_NAME_H_
#define PATHNAME_MGMT_REC_NAME_H_

#include "../../../defn/rec/Size.h"
#include "../../../intf/Record.h"
#include "../../../intf/File.h"

#include <cstdint>
#include <cstddef>
#include <string>

namespace pathname
{
namespace mgmt
{
namespace rec
{

class Name :
    public ::intf::Record
{
public:
    typedef ::intf::Record RecordInterfaceType;
public:
    typedef typename RecordInterfaceType::OffsetType OffsetType;
    typedef typename RecordInterfaceType::PositionType PositionType;
    typedef typename RecordInterfaceType::SizeType SizeType;
    typedef typename RecordInterfaceType::WayType WayType;
public:
    typedef typename RecordInterfaceType::InputType InputType;
    typedef typename RecordInterfaceType::OutputType OutputType;
public:
    typedef typename RecordInterfaceType::StatusValueType StatusValueType;
    typedef typename RecordInterfaceType::StatusType StatusType;
public:
    typedef intf::File FileInterfaceType;
    typedef typename FileInterfaceType::PositionType FilePositionType;
private:
    typedef std::uint8_t SyncType;
    typedef std::uint8_t FlagsValueType;
public:
    static constexpr std::size_t ms_flags_alloc_size = sizeof(FlagsValueType);
    static constexpr std::size_t ms_pathname_alloc_size = 256;
public:
    static constexpr std::size_t ms_delete_flag = 0x01;
private:
    static constexpr SyncType ms_flags_sync = 0x01;
    static constexpr SyncType ms_pathname_sync = 0x02;
public:
    static constexpr std::size_t Size();
private:
    SyncType m_sync_flags;
    FlagsValueType m_flags;
    std::string m_pathname;
public:
    Name();
    Name(const std::string & pathname);
public:
    Name(const Name & cpy);
    Name(Name && mov);
public:
    Name & operator=(const Name & cpy);
    Name & operator=(Name && mov);
public:
    bool IsDelete() const;
    void Delete();
public:
    std::string Value() const;
    void Value(const std::string & pathname);
public:
    SizeType Put(OutputType & out) const;
public:
    SizeType Get(InputType & in);
public:
    bool operator==(const RecordInterfaceType & rec) const;
    bool operator!=(const RecordInterfaceType & rec) const;
};

constexpr std::size_t Name::Size()
{
    return ms_flags_alloc_size + ms_pathname_alloc_size;
}

} //!rec

} //!mgmt

} //!pathname

template<>
struct defn::rec::Size<pathname::mgmt::rec::Name>
{
    typedef std::size_t ValueType;
    static constexpr ValueType Value = pathname::mgmt::rec::Name::Size();
};

#endif //!PATHNAME_MGMT_REC_NAME_H_
