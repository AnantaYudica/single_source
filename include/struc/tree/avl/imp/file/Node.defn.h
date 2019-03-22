#ifndef STRUC_TREE_AVL_IMP_FILE_NODE_DEFN_H_
#define STRUC_TREE_AVL_IMP_FILE_NODE_DEFN_H_

#include "Node.decl.h"
#include "Pointer.decl.h"
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
    typedef ::intf::File FileInterfaceType;
    typedef typename FileInterfaceType::PointerType 
        FileInterfacePointerType;
public:
    typedef typename FileInterfaceType::PositionType FilePositionType;
    typedef typename FileInterfaceType::OffsetType FileOffsetType;
    typedef typename FileInterfaceType::SizeType FileSizeType;
public:
    typedef struc::tree::avl::intf::Node<TData> NodeInterfaceType;
    typedef typename NodeInterfaceType::NodePointerType 
        NodeInterfacePointerType;
    typedef struc::tree::avl::intf::Pointer<TData> PointerInterfaceType;
public:
    typedef TData DataType;
public:
    typedef std::uint8_t FlagValueType;
    typedef std::uint32_t HightValueType;
    typedef FilePositionType PositionType;
private:
    typedef std::uint8_t FlagReadWrite;
private:
    constexpr static std::size_t ms_flagOffset = 0;
    constexpr static std::size_t ms_hightOffset = sizeof(FlagValueType);
    constexpr static std::size_t ms_parentPositionOffset =
        ms_hightOffset + sizeof(HightValueType);
    constexpr static std::size_t ms_rightPositionOffset =
        ms_parentPositionOffset + sizeof(PositionType);
    constexpr static std::size_t ms_leftPositionOffset =
        ms_rightPositionOffset + sizeof(PositionType);
    constexpr static std::size_t ms_dataOffset =
        ms_leftPositionOffset + sizeof(PositionType);
    constexpr static std::size_t ms_endOffset = 
        ms_dataOffset + sizeof(TData);
private:
    constexpr static FlagReadWrite ms_rwFlag = 0x01;
    constexpr static FlagReadWrite ms_rwHight = 0x02;
    constexpr static FlagReadWrite ms_rwParentPosition = 0x04;
    constexpr static FlagReadWrite ms_rwRightPosition = 0x08;
    constexpr static FlagReadWrite ms_rwLeftPosition = 0x10;
    constexpr static FlagReadWrite ms_rwData = 0x20;
private:
    FlagValueType m_flag;
    HightValueType m_hight;
    PositionType m_parentPosition;
    PositionType m_rightPosition;
    PositionType m_leftPosition;
    FilePositionType m_position;
    FileInterfacePointerType m_file;
    PointerType * m_parent;
    PointerType * m_right;
    PointerType * m_left;
    TData m_data;
public:
    Node();
    Node(FileInterfacePointerType file);
    Node(FileInterfacePointerType file, FilePositionType position);
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
private:
    void Default();
private:
    template<typename TValue>
    TValue ReadValue(FilePositionType position);
    template<typename TValue>
    void WriteValue(const TValue & val, FilePositionType position);
private:
    void Read(std::uint8_t flags = 0xFF);
    void Write(std::uint8_t flags = 0xFF);
public:
    Node<TData> Instance(FilePositionType position = -1) const;
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
    void Parent(const PositionType & new_position);
public:
    PointerInterfaceType & Right();
    const PointerInterfaceType & Right() const;
    void Right(const PositionType & new_position);
public:
    PointerInterfaceType & Left();
    const PointerInterfaceType & Left() const;
    void Left(const PositionType & new_position);
public:
    int Hight();
    int Hight() const;
    int Hight(int & set);
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
    TData & operator*();
    const TData & operator*() const;
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
