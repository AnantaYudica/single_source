#include "mgmt/pathname/rec/Tree.h"

#include <utility>
#include <string>

using namespace std;
using namespace mgmt::pathname::rec;

Tree::Tree() :
    ::intf::Record(),
    m_sync_flags(0),
    m_position(-1),
    m_pathname()
{}

Tree::Tree(const FilePositionType & pos) :
    ::intf::Record(),
    m_sync_flags(0),
    m_position(pos),
    m_pathname()
{}

Tree::Tree(const std::string & pathname) :
    ::intf::Record(),
    m_sync_flags(0),
    m_position(-1),
    m_pathname(pathname)
{}

Tree::~Tree()
{
    m_sync_flags = 0;
    m_position = -1;
}

Tree::Tree(const Tree & cpy) :
    ::intf::Record(cpy),
    m_sync_flags(cpy.m_sync_flags),
    m_position(cpy.m_position),
    m_pathname(cpy.m_pathname)
{}

Tree::Tree(Tree && mov) :
    ::intf::Record(move(mov)),
    m_sync_flags(mov.m_sync_flags),
    m_position(mov.m_position),
    m_pathname(move(mov.m_pathname))
{
    mov.m_sync_flags = 0;
    mov.m_position = -1;
    mov.m_pathname.clear();
}

Tree & Tree::operator=(const Tree & cpy)
{
    ::intf::Record::operator=(cpy);
    m_sync_flags = cpy.m_sync_flags;
    m_position = cpy.m_position;
    m_pathname = cpy.m_pathname;
    return *this;
}

Tree & Tree::operator=(Tree && mov)
{
    ::intf::Record::operator=(move(mov));
    m_sync_flags = mov.m_sync_flags;
    m_position = mov.m_position;
    m_pathname = move(mov.m_pathname);
    mov.m_sync_flags = 0;
    mov.m_position = -1;
    mov.m_pathname.clear();
    return *this;
}

typename Tree::FilePositionType Tree::Position() const
{
    return m_position;
}

void Tree::Position(const FilePositionType & pos)
{
    if (m_position == pos)return;
    m_position = pos;
    m_sync_flags |= ms_position_sync;
    OutOfSynchronization(*this);
}

string Tree::Pathname() const
{
    return m_pathname;
}

void Tree::Pathname(const string & pathname)
{
    if (m_pathname.compare(pathname) == 0) return;
    m_pathname = pathname;
}

typename Tree::SizeType Tree::Put(OutputType & out) const
{
    if (IsInitial() || (m_sync_flags & ms_position_sync))
    {
        if (out.CurrentPut((const char *)&m_position, 
            sizeof(FilePositionType)) == 0) return Bad<SizeType>(*this, 0);
        if (m_sync_flags & ms_position_sync)
            const_cast<SyncType &>(m_sync_flags) ^= ms_position_sync;
    }
    out.SeekOffset(sizeof(FilePositionType), WayType::current);
    return Good<SizeType>(*this, sizeof(FilePositionType));
}

typename Tree::SizeType Tree::Get(InputType & in)
{
    if (IsInitial() || (m_sync_flags & ms_position_sync))
    {
        if (in.CurrentGet((char *)&m_position, sizeof(FilePositionType)) == 0)
            return Bad<SizeType>(*this, 0);
        if (m_sync_flags & ms_position_sync)
            m_sync_flags ^= ms_position_sync;
    }
    in.SeekOffset(sizeof(FilePositionType), WayType::current);
    return Good<SizeType>(*this, sizeof(FilePositionType));
}

bool Tree::operator==(const RecordInterfaceType & rec) const
{
    auto rec_tree = dynamic_cast<Tree const*>(&rec);
    if (!rec_tree) return false;
    if (rec_tree->m_position == m_position)
        return true;
    if (rec_tree->m_position != -1 ||
        m_position != -1) return false;
    return m_pathname.compare(rec_tree->m_pathname) == 0;
}

bool Tree::operator!=(const RecordInterfaceType & rec) const
{
    return !(*this == rec);
}
