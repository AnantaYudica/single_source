#ifndef STRUC_TREE_AVL_IMP_FILE_NODE_DEFN_H_
#define STRUC_TREE_AVL_IMP_FILE_NODE_DEFN_H_

#include "Node.decl.h"
#include "Pointer.decl.h"
#include "node/Record.h"
#include "../../../../../file/format/Linear.h"
#include "../../../../../intf/File.h"

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

template<typename TData>
class Node :
    public struc::tree::avl::intf::Node<TData>
{
public:
    typedef node::Record<TData> RecordType;
    typedef typename RecordType::FlagsValueType FlagsValueType;
    typedef typename RecordType::HightValueType HightValueType;
public:
    typedef ::intf::File FileInterfaceType;
    typedef typename FileInterfaceType::PointerType 
        FileInterfacePointerType;
private:
    typedef ::file::format::Linear FileFormatLinearType;
    typedef typename FileFormatLinearType::PointerType
        FileFormatLinearPointerType;
public:
    typedef typename FileInterfaceType::PositionType PositionType;
    typedef typename FileInterfaceType::OffsetType OffsetType;
    typedef typename FileInterfaceType::SizeType SizeType;
public:
    typedef typename FileInterfaceType::WayType WayType;
    typedef typename FileInterfaceType::ModeType ModeType;
    typedef typename FileInterfaceType::ModeValueType ModeValueType;
public:
    typedef struc::tree::avl::intf::Node<TData> NodeInterfaceType;
    typedef typename NodeInterfaceType::PointerType 
        NodeInterfacePointerType;
public: 
    typedef struc::tree::avl::intf::Pointer<TData> PointerInterfaceType;
public:
    typedef TData DataType;
private:
    PositionType m_position;
    FileFormatLinearPointerType m_fileFormatLinear;
    Pointer<TData> * m_parent;
    Pointer<TData> * m_right;
    Pointer<TData> * m_left;
    FileInterfacePointerType m_file;
    RecordType m_record;
public:
    Node();
    Node(FileInterfacePointerType file, FileFormatLinearPointerType  format);
    Node(FileInterfacePointerType file, FileFormatLinearPointerType  format, 
        const PositionType & position);
public:
    ~Node(); 
public:
    Node(const Node<TData> & cpy);
    Node(Node<TData> && mov);
public:
    Node<TData> & operator=(const Node<TData> & cpy);
    Node<TData> & operator=(Node<TData> && mov);
    Node<TData> & operator=(const NodeInterfaceType & cpy);
    Node<TData> & operator=(NodeInterfaceType && mov);
public:
    Node<TData> & operator=(const TData & data);
private:
    void Default();
private:
    bool IsValid() const;
public:
    Node<TData> Instance(PositionType position = -1) const;
private:
    NodeInterfacePointerType MakeCopy() const;
    NodeInterfacePointerType MakeMove();
public:
    Node<TData> & Emplace(const TData & data);
public:
    Node<TData> & Displace();
public:
    PositionType Position() const;
public:
    PointerInterfaceType & Parent();
    const PointerInterfaceType & Parent() const;
public:
    PositionType ParentPosition() const;
    void ParentPosition(const PositionType & new_position);
public:
    PointerInterfaceType & Right();
    const PointerInterfaceType & Right() const;
public:
    PositionType RightPosition() const;
    void RightPosition(const PositionType & new_position);
public:
    PointerInterfaceType & Left();
    const PointerInterfaceType & Left() const;
public:
    PositionType LeftPosition() const;
    void LeftPosition(const PositionType & new_position);
public:
    int Hight() const;
    int Hight(const HightValueType & set);
public:
    int Balance();
    int Balance() const;
public:
    void Swap(NodeInterfaceType & other);
public:
    bool IsDeleted();
public:
    void Synchronize();
public:
    TData operator*() const;
public:
    bool operator==(const NodeInterfaceType & other) const;
    bool operator!=(const NodeInterfaceType & other) const;
public:
    operator bool() const;
};

} //file

} //!imp

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_IMP_FILE_NODE_DEFN_H_
