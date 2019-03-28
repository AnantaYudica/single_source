#ifndef STRUC_TREE_AVL_IMP_FILE_NODE_RECORD_H_
#define STRUC_TREE_AVL_IMP_FILE_NODE_RECORD_H_

#include "../../../../../../defn/rec/Size.h"
#include "../../../../../../intf/Record.h"
#include "../../../../../../intf/File.h"

#include <utility>
#include <cstdint>

namespace struc
{
namespace tree
{
namespace avl
{
namespace imp
{
namespace file
{
namespace node
{

template<typename TData>
class Record :
    public intf::Record
{
public:
    typedef ::intf::Record RecordInterfaceType;
    typedef std::uint8_t FlagsValueType;
    typedef std::uint32_t HightValueType;
    typedef typename RecordInterfaceType::InputType InputType;
    typedef typename RecordInterfaceType::OutputType OutputType;
    typedef typename RecordInterfaceType::StatusValueType StatusValueType;
    typedef typename RecordInterfaceType::StatusType StatusType;
    typedef typename RecordInterfaceType::PositionType PositionType;
    typedef typename RecordInterfaceType::OffsetType OffsetType;
    typedef typename RecordInterfaceType::SizeType SizeType;
    typedef typename RecordInterfaceType::WayType WayType;
private:
    typedef std::uint8_t SyncType;
private:
    static constexpr SyncType ms_flags_sync = 0x01;
    static constexpr SyncType ms_hight_sync = 0x02;
    static constexpr SyncType ms_parent_sync = 0x04;
    static constexpr SyncType ms_right_sync = 0x08;
    static constexpr SyncType ms_left_sync = 0x10;
    static constexpr SyncType ms_current_sync = 0x20;
    static constexpr SyncType ms_data_sync = 0x40;
public:
    static constexpr FlagsValueType delete_flag = 0x01;
private:
    FlagsValueType m_flags;
    HightValueType m_hight;
    PositionType m_parent;
    PositionType m_right;
    PositionType m_left;
    PositionType m_current;
    TData m_data;
    SyncType m_sync_flags;
    StatusValueType m_status;
public:
    static constexpr std::size_t Size();
private:
    static void Default(Record<TData> & rec);
private:
    template<typename TReturn>
    static TReturn Bad(Record<TData> & rec, TReturn val);
private:
    template<typename TValue>
    static SizeType Put(Record<TData> & rec, OutputType & out, 
        const TValue & val, SyncType & sync_flags, SyncType sync_index_val);
    static SizeType Put(Record<TData> & rec, OutputType & out, 
        const ::intf::Record & intf_rec, SyncType & sync_flags, 
        SyncType sync_index_val);
private:
    template<typename TValue>
    static SizeType Get(Record<TData> & rec, InputType & in, TValue & val, 
        SyncType & sync_flags, SyncType sync_index_val);
    static SizeType Get(Record<TData> & rec, InputType & in, 
        ::intf::Record & intf_rec, SyncType & sync_flags, 
        SyncType sync_index_val);
public:
    Record();
public:
    Record(const Record<TData> & cpy);
    Record(Record<TData> && mov);
public:
    Record<TData> & operator=(const Record<TData> & cpy);
    Record<TData> & operator=(Record<TData> && mov);
public:
    const FlagsValueType & Flags() const;
    void Flags(const FlagsValueType & flags);
public:
    const HightValueType & Hight() const;
    void Hight(const HightValueType & hight);
public:
    const PositionType & Parent() const;
    void Parent(const PositionType & parent);
public:
    const PositionType & Right() const;
    void Right(const PositionType & right);
public:
    const PositionType & Left() const;
    void Left(const PositionType & left);
public:
    const PositionType & Current() const;
    void Current(const PositionType & curr);
public:
    const TData & Data() const;
    void Data(const TData & data);
public:
    SizeType Put(OutputType & out) const;
public:
    SizeType Get(InputType & in);
public:
    StatusValueType Status() const;
public:
    bool operator==(const RecordInterfaceType & rec_intf) const;
    bool operator!=(const RecordInterfaceType & rec_intf) const;
};

template<typename TData>
constexpr std::size_t Record<TData>::Size()
{
    return sizeof(FlagValueType) +
        sizeof(HightValueType) +
        sizeof(FilePositionType) +
        sizeof(FilePositionType) +
        sizeof(FilePositionType) +
        sizeof(FilePositionType) +
        defn::rec::Size<TData>::Value;
}

template<typename TData>
void Record<TData>::Default(Record<TData> & rec)
{
    rec.m_flags = 0;
    rec.m_hight = 0;
    rec.m_parent = -1;
    rec.m_right = -1;
    rec.m_left = -1;
    rec.m_current = -1;
    rec.m_data = TData();
    rec.m_sync_flags = 0;
    rec.m_status = (StatusValueType)StatusType::initial;
}

template<typename TData>
template<typename TReturn>
TReturn Record<TData>::Bad(Record<TData> & rec, TReturn val)
{
    rec.m_status = (StatusValueType)StatusType::bad;
    return val;
}

template<typename TData>
template<typename TValue>
typename Record<TData>::SizeType Record<TData>::Put(Record<TData> & rec,
    OutputType & out, const TValue & val, SyncType & sync_flags, 
    SyncType sync_index_val)
{
    if (sync_flags & sync_index_val)
    {
        if (!out.Put((const char *)&val, sizeof(TValue)))
            return Bad(rec, 0);
        sync_flags ^= sync_index_val;
    }
    else
        out.SeekOffset(sizeof(TValue), WayType::current);
    return sizeof(TValue);
}

template<typename TData>
typename Record<TData>::SizeType Record<TData>::Put(Record<TData> & rec,
    OutputType & out, const ::intf::Record & intf_rec, SyncType & sync_flags, 
    SyncType sync_index_val)
{
    if (sync_flags & sync_index_val)
    {
        if (!intf_rec.Put(out)) return Bad(rec, 0);
        sync_flags ^= sync_index_val;
    }
    else
        out.SeekOffset(defn::rec::Size<TData>::Value, WayType::current);
    return defn::rec::Size<TData>::Value;
}

template<typename TData>
template<typename TValue>
typename Record<TData>::SizeType Record<TData>::Get(Record<TData> & rec,
    InputType & in, TValue & val, SyncType & sync_flags, 
    SyncType sync_index_val)
{
    char * buffer = reinterpret_cast<char *>(&val);
    if (!in.Get(buffer, sizeof(TValue))) return Bad(rec, 0);
    if (sync_flags & sync_index_val)
        sync_flags ^= sync_index_val;
    return sizeof(TValue);
}

template<typename TData>
typename Record<TData>::SizeType Record<TData>::Get(Record<TData> & rec,
    InputType & in, ::intf::Record & intf_rec, SyncType & sync_flags, 
    SyncType sync_index_val)
{
    if (!intf_rec.Get(in)) return Bad(rec, 0);
    if (sync_flags & sync_index_val)
        sync_flags ^= sync_index_val;
    return defn::rec::Size<TData>::Value;
}

template<typename TData>
Record<TData>::Record() :
    m_flags(0),
    m_hight(0),
    m_parent(-1),
    m_right(-1),
    m_left(-1),
    m_current(-1),
    m_data(),
    m_sync_flags(0),
    m_status((StatusValueType)StatusType::initial)
{}

template<typename TData>
Record<TData>::Record(const Record<TData> & cpy) :
    m_flags(cpy.m_flags),
    m_hight(cpy.m_hight),
    m_parent(cpy.m_parent),
    m_right(cpy.m_right),
    m_left(cpy.m_left),
    m_current(cpy.m_current),
    m_data(cpy.m_data),
    m_sync_flags(cpy.m_sync_flags),
    m_status(cpy.m_status)
{}

template<typename TData>
Record<TData>::Record(Record<TData> && mov) :
    m_flags(std::move(mov.m_flags)),
    m_hight(std::move(mov.m_hight)),
    m_parent(std::move(mov.m_parent)),
    m_right(std::move(mov.m_right)),
    m_left(std::move(mov.m_left)),
    m_current(std::move(mov.m_current)),
    m_data(std::move(mov.m_data)),
    m_sync_flags(std::move(mov.m_sync_flags)),
    m_status(std::move(mov.m_status))
{
    Default(mov);
}

template<typename TData>
Record<TData> & Record<TData>::operator=(const Record<TData> & cpy)
{
    m_flags = cpy.m_flags;
    m_hight = cpy.m_hight;
    m_parent = cpy.m_parent;
    m_right = cpy.m_right;
    m_left = cpy.m_left;
    m_current = cpy.m_current;
    m_data = cpy.m_data;
    m_sync_flags = cpy.m_sync_flags;
    m_status = cpy.m_status;
    return *this;
}

template<typename TData>
Record<TData> & Record<TData>::operator=(Record<TData> && mov)
{
    m_flags = std::move(mov.m_flags);
    m_hight = std::move(mov.m_hight);
    m_parent = std::move(mov.m_parent);
    m_right = std::move(mov.m_right);
    m_left = std::move(mov.m_left);
    m_current = std::move(mov.m_current);
    m_data = std::move(mov.m_data);
    m_sync_flags = std::move(mov.m_sync_flags);
    m_status = std::move(mov.m_status);
    Default(mov);
    return *this;
}

template<typename TData>
const typename Record<TData>::FlagsValueType & Record<TData>::Flags() const
{
    return m_flags;
}

template<typename TData>
void Record<TData>::Flags(const Record<TData>::FlagsValueType & flags)
{
    if (flags == m_flags) return;
    m_flags = flags;
    m_sync_flags |= ms_flags_sync;
    m_status |= (StatusValueType)StatusType::out_sync;
}

template<typename TData>
const typename Record<TData>::HightValueType & Record<TData>::Hight() const
{
    return m_hight;
}

template<typename TData>
void Record<TData>::Hight(const HightValueType & hight)
{
    if (hight == m_hight) return;
    m_hight = hight;
    m_sync_flags |= ms_hight_sync;
    m_status |= (StatusValueType)StatusType::out_sync;
}

template<typename TData>
const typename Record<TData>::PositionType & Record<TData>::Parent() const
{
    return m_parent;
}

template<typename TData>
void Record<TData>::Parent(const PositionType & parent)
{
    if (parent == m_parent) return;
    m_parent = parent;
    m_sync_flags |= ms_parent_sync;
    m_status |= (StatusValueType)StatusType::out_sync;
}

template<typename TData>
const typename Record<TData>::PositionType & Record<TData>::Right() const
{
    return m_right;
}

template<typename TData>
void Record<TData>::Right(const PositionType & right)
{
    if (parent == m_parent) return;
    m_parent = parent;
    m_sync_flags |= ms_parent_sync;
    m_status |= (StatusValueType)StatusType::out_sync;
}

template<typename TData>
const typename Record<TData>::PositionType & Record<TData>::Left() const
{
    return m_left;
}

template<typename TData>
void Record<TData>::Left(const PositionType & left)
{
    if (left == m_left) return;
    m_left = left;
    m_sync_flags |= ms_left_sync;
    m_status |= (StatusValueType)StatusType::out_sync;
}

template<typename TData>
const Record<TData>::PositionType & Record<TData>::Current() const
{
    return m_current;
}

template<typename TData>
void Record<TData>::Current(const PositionType & curr)
{
    if (curr == m_current) return;
    m_current = curr;
    m_sync_flags |= ms_current_sync;
    m_status |= (StatusValueType)StatusType::out_sync;
}

template<typename TData>
const TData & Record<TData>::Data() const
{
    return m_data;
}

template<typename TData>
void Record<TData>::Data(const TData & data)
{
    if (data == m_data) return;
    m_data = data;
    m_sync_flags |= ms_data_sync;
    m_status |= (StatusValueType)StatusType::out_sync;
}

template<typename TData>
typename Record<TData>::SizeType Record<TData>::Put(OutputType & out) const
{
    if (!out.IsOpen()) return Bad(*this, 0);
    if (!Put(*this, out, m_flags, m_sync_flags, ms_flags_sync))
        return 0;
    if (!Put(*this, out, m_hight, m_sync_flags, ms_hight_sync))
        return 0;
    if (!Put(*this, out, m_parent, m_sync_flags, ms_parent_sync))
        return 0;
    if (!Put(*this, out, m_right, m_sync_flags, ms_right_sync))
        return 0;
    if (!Put(*this, (out, m_left, m_sync_flags, ms_left_sync))
        return 0;
    if (!Put(*this, (out, m_current, m_sync_flags, ms_current_sync))
        return 0;
    if (!Put(*this, put, m_data, m_sync_flags, ms_data_sync))
        return 0;
    if (m_status & (StatusValueType)StatusType::out_sync)
        m_status ^= (StatusValueType)StatusType::out_sync;
    return Size();
}

template<typename TData>
typename Record<TData>::SizeType Record<TData>::Get(InputType & in)
{
    if (!out.IsOpen()) return Bad(*this, 0);
    if (!Get(*this, out, m_flags, m_sync_flags, ms_flags_sync))
        return 0;
    if (!Get(*this, out, m_hight, m_sync_flags, ms_hight_sync))
        return 0;
    if (!Get(*this, out, m_parent, m_sync_flags, ms_parent_sync))
        return 0;
    if (!Get(*this, out, m_right, m_sync_flags, ms_right_sync))
        return 0;
    if (!Get*this, (out, m_left, m_sync_flags, ms_left_sync))
        return 0;
    if (!Get(*this, put, m_data, m_sync_flags, ms_data_sync))
        return 0;
    m_status = (StatusValueType)StatusType::good;
    return Size();
}

template<typename TData>
typename Record<TData>::StatusValueType Record<TData>::Status() const
{
    return m_status;
}

template<typename TData>
bool Record<TData>::operator==(const RecordInterfaceType & rec_intf) const
{
    const Record<TData> * rec = dynamic_cast<const Record<TData> *>(&rec_intf);
    if (!rec) return false;
    return m_sync_flags == rec->m_sync_flags && 
        m_status == rec->m_status &&
        m_flags == rec->m_flags &&
        m_hight == rec->m_hight &&
        m_left == rec->m_left &&
        m_right == rec->m_right &&
        m_current == rec->m_current &&
        m_parent == rec->m_parent &&
        m_data == rec->m_data;
}

template<typename TData>
bool Record<TData>::operator!=(const RecordInterfaceType & rec_intf) const
{
    return !(*this == rec_intf);
}

} //!node

} //!file

} //!imp

} //!avl

} //!tree

} //!struc

template<typename TData>
struct defn::rec::Size<struc::tree::avl::imp::file::node::Record<TData>>
{
    typedef std::size_t ValueType;
    static constexpr ValueType Value = struc::tree::avl::imp::file::node::
        Record<TData>::Size();
};


#endif //!STRUC_TREE_AVL_IMP_FILE_NODE_RECORD_H_
