#include "mgmt/pathname/rec/Linear.h"

#include <utility>
#include <cstring>

using namespace std;
using namespace mgmt::pathname::rec;

Linear::Linear() :
    ::intf::Record(),
    m_sync_flags(0),
    m_flags(0),
    m_pathname()
{}

Linear::Linear(const std::string & pathname) :
    ::intf::Record(),
    m_sync_flags(ms_pathname_sync),
    m_flags(0),
    m_pathname(pathname)
{}

Linear::Linear(const Linear & cpy) :
    ::intf::Record(cpy),
    m_sync_flags(cpy.m_sync_flags),
    m_flags(cpy.m_flags),
    m_pathname(cpy.m_pathname)
{}

Linear::Linear(Linear && mov) :
    ::intf::Record(move(mov)),
    m_sync_flags(move(mov.m_sync_flags)),
    m_flags(move(mov.m_flags)),
    m_pathname(move(mov.m_pathname))
{
    mov.m_sync_flags = 0;
    mov.m_flags = 0;
    mov.m_pathname.clear();
}

Linear & Linear::operator=(const Linear & cpy)
{
    ::intf::Record::operator=(cpy);
    m_sync_flags = cpy.m_sync_flags;
    m_flags = cpy.m_flags;
    m_pathname = cpy.m_pathname;
    return *this;
}

Linear & Linear::operator=(Linear && mov)
{
    ::intf::Record::operator=(move(mov));
    m_sync_flags = move(mov.m_sync_flags);
    m_flags = move(mov.m_flags);
    m_pathname = move(mov.m_pathname);
    mov.m_sync_flags = 0;
    mov.m_flags = 0;
    mov.m_pathname.clear();
    return *this;
}

bool Linear::IsDelete() const
{
    return m_flags & ms_delete_flag;
}

void Linear::Delete()
{
    m_flags |= ms_delete_flag;
    m_sync_flags |= ms_flags_sync;
    OutOfSynchronization(*this);
}

string Linear::Value() const
{
    return m_pathname;
}

void Linear::Value(const string & pathname)
{
    m_pathname = pathname;
    m_sync_flags |= ms_pathname_sync;
    OutOfSynchronization(*this);
}

typename Linear::SizeType Linear::Put(OutputType & out) const
{
    if (m_sync_flags & ms_flags_sync)
    {
        if (!out.Put((const char *)&m_flags, sizeof(FlagsValueType)))
            return Bad<SizeType>(*this, 0);
        const_cast<SyncType &>(m_sync_flags) ^= ms_flags_sync;
    }
    else
        out.SeekOffset(sizeof(FlagsValueType), WayType::current);
    if (m_sync_flags & ms_pathname_sync)
    {
        const auto size = m_pathname.size() < ms_pathname_alloc_size ?
            m_pathname.size() : ms_pathname_alloc_size;
        if (!out.Put(m_pathname.c_str(), size))
            return Bad<SizeType>(*this, 0);
        const_cast<SyncType &>(m_sync_flags) ^= ms_pathname_sync;
        out.SeekOffset(ms_pathname_alloc_size - size, WayType::current);
    }
    else
        out.SeekOffset(ms_pathname_alloc_size, WayType::current);
    return Good<SizeType>(*this, 
        sizeof(FilePositionType));
}

typename Linear::SizeType Linear::Get(InputType & in)
{
    if (m_sync_flags & ms_flags_sync)
    {
        if (!in.Get((char *)&m_flags, sizeof(FlagsValueType)))
            return Bad<SizeType>(*this, 0);
        m_sync_flags ^= ms_flags_sync;
    }
    else
        in.SeekOffset(sizeof(FlagsValueType), WayType::current);
    if (m_sync_flags & ms_pathname_sync)
    {
        char buffer[ms_pathname_alloc_size + 1];
        auto size = in.Get(buffer, sizeof(ms_pathname_alloc_size));
        if (size == 0) return Bad<SizeType>(*this, 0);
        buffer[ms_pathname_alloc_size] = '\0';
        m_pathname = buffer;
        m_sync_flags ^= ms_pathname_sync;
        in.SeekOffset(ms_pathname_alloc_size - size, WayType::current);
    }
    else
        in.SeekOffset(ms_pathname_alloc_size, WayType::current);
    return Good<SizeType>(*this, 
        sizeof(FilePositionType));
}

bool Linear::operator==(const RecordInterfaceType & rec) const
{
    auto * rec_name = dynamic_cast<const Linear *>(&rec);
    if (!rec_name) return false;
    return m_flags == rec_name->m_flags &&
        m_pathname.compare(rec_name->m_pathname) == 0;
}

bool Linear::operator!=(const RecordInterfaceType & rec) const
{
    return !(*this == rec);
}