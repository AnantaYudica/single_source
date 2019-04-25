#ifndef MGMT_PATHNAME_REC_LINEAR_H_
#define MGMT_PATHNAME_REC_LINEAR_H_

#include "../../../defn/rec/Size.h"
#include "../../../intf/Record.h"
#include "../../../intf/File.h"

#include <cstdint>
#include <cstddef>
#include <string>

namespace mgmt
{
namespace pathname
{
namespace rec
{

class Linear :
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
private:
    SyncType m_sync_flags;
    FlagsValueType m_flags;
    std::string m_pathname;
public:
    Linear();
    Linear(const std::string & pathname);
    ~Linear();
public:
    Linear(const Linear & cpy);
    Linear(Linear && mov);
public:
    Linear & operator=(const Linear & cpy);
    Linear & operator=(Linear && mov);
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

} //!rec

} //!pathname

} //!mgmt

template<>
struct defn::rec::Size<mgmt::pathname::rec::Linear>
{
    typedef std::size_t ValueType;
    static constexpr ValueType Value = 
        ::mgmt::pathname::rec::Linear::ms_flags_alloc_size + 
        ::mgmt::pathname::rec::Linear::ms_pathname_alloc_size + 1;
};

#endif //!MGMT_PATHNAME_REC_LINEAR_H_
