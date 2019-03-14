#ifndef FILE_FORMAT_TREE_AVL_NODE_DEFN_H_
#define FILE_FORMAT_TREE_AVL_NODE_DEFN_H_

#include "Node.decl.h"
#include "Pointer.decl.h"

#include <fstream>
#include <cstdint>

namespace file
{
namespace format
{
namespace tree
{
namespace avl
{

template<typename TData>
class Node :
    public struc::tree::avl::intf::Node<TData>
{
public:
    typedef struc::tree::avl::intf::Node<TData> NodeInterfaceType;
    typedef struc::tree::avl::intf::Pointer<TData> PointerInterfaceType;
    typedef Pointer<TData> PointerType;
public:
    typedef TData DataType;
public:
    typedef std::uint8_t FlagValueType;
    typedef std::uint32_t HightValueType;
    typedef std::uint64_t PositionType;
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
    std::streampos m_position;
    FlagValueType m_flag;
    HightValueType m_hight;
    PositionType m_parentPosition;
    PositionType m_rightPosition;
    PositionType m_leftPosition;
    std::filebuf * m_filebuffer;
    PointerType * m_parent;
    PointerType * m_right;
    PointerType * m_left;
    TData m_data;
public:
    Node();
    Node(std::filebuf * filebuffer);
    Node(std::filebuf * filebuffer, std::streampos position);
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
    TValue ReadValue(std::streampos position);
    template<typename TValue>
    void WriteValue(const TValue & val, std::streampos position);
private:
    void Read(std::uint8_t flags = 0xFF);
    void Write(std::uint8_t flags = 0xFF);
public:
    Node<TData> Instance(std::streampos position = -1) const;
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

} //!avl

} //!tree

} //!format

} //!file

#endif //!FILE_FORMAT_TREE_AVL_NODE_DEFN_H_
