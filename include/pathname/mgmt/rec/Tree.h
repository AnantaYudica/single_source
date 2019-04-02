#ifndef PATHNAME_MGMT_REC_TREE_H_
#define PATHNAME_MGMT_REC_TREE_H_

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

class Tree :
    public ::intf::Record
{
public:
    typedef intf::Record RecordInterfaceType;
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
private:
    static constexpr SyncType ms_position_sync = 0x01;
public:
    static constexpr std::size_t Size();
private:
    SyncType m_sync_flags;
    FilePositionType m_position;
    std::string m_pathname;
public:
    Tree();
    Tree(const FilePositionType & pos);
    Tree(const std::string & pathname);
public:
    Tree(const Tree & cpy);
    Tree(Tree && mov);
public:
    Tree & operator=(const Tree & cpy);
    Tree & operator=(Tree && mov);
public:
    FilePositionType Position() const;
    void Position(const FilePositionType & pos);
public:
    std::string Pathname() const;
    void Pathname(const std::string & pathname);
public:
    SizeType Put(OutputType & out) const;
public:
    SizeType Get(InputType & in);
public:
    bool operator==(const RecordInterfaceType & rec) const;
    bool operator!=(const RecordInterfaceType & rec) const;
};

constexpr std::size_t Tree::Size()
{
    return sizeof(FilePositionType);
}

} //rec

} //!mgmt

} //!pathname

template<>
struct defn::rec::Size<pathname::mgmt::rec::Tree>
{
    typedef std::size_t ValueType;
    static constexpr ValueType Value = pathname::mgmt::rec::Tree::Size();
};

#endif //!PATHNAME_MGMT_REC_TREE_H_
