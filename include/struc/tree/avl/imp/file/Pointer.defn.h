#ifndef FILE_FORMAT_TREE_AVL_POINTER_DEFN_H_
#define FILE_FORMAT_TREE_AVL_POINTER_DEFN_H_

#include "Pointer.decl.h"
#include "Node.decl.h"

namespace file
{
namespace format
{
namespace tree
{
namespace avl
{

template<typename TData>
class Pointer :
    public struc::tree::avl::intf::Pointer<TData>
{
public:
    enum class Way
    {
        undefined,
        parent,
        right,
        left
    };
public:
    typedef TData DataType;
    typedef struc::tree::avl::intf::Pointer<DataType> PointerInterfaceType;
    typedef struc::tree::avl::intf::Node<DataType> NodeInterfaceType; 
    typedef Node<DataType> NodeType;
    typedef typename NodeType::PositionType PositionType;
private:
    NodeType * m_point;
    NodeType * m_next;
    Way m_way;
public:
    Pointer();
    Pointer(NodeType * base, Way way);
public:
    ~Pointer(); 
public:
    Pointer(const Pointer<TData> & cpy);
    Pointer(Pointer<TData> && mov);
public:
    Pointer<TData> & operator=(const Pointer<TData> & cpy);
    Pointer<TData> & operator=(Pointer<TData> && mov);
    Pointer<TData> & operator=(const PointerInterfaceType & cpy);
    Pointer<TData> & operator=(PointerInterfaceType && mov);
private:
    void Default();
private:
    void Set(const PositionType & pos);
    PositionType Get();
private:
    NodeType * Node();
private:
    void Reset();
private:
    bool IsNull() const;
public:
    PointerInterfaceType & Emplace(const DataType & data);
public:
    PointerInterfaceType & Displace();
public:
    Pointer<TData> & operator=(const NodeInterfaceType & other);
public:
    NodeInterfaceType & operator*();
    const NodeInterfaceType & operator*() const;
public:
    NodeInterfaceType * operator->();
    const NodeInterfaceType * operator->() const;
public:
    operator bool() const;
public:
    bool operator==(const PointerInterfaceType & other) const;
    bool operator==(const NodeInterfaceType & other) const;
    bool operator!=(const PointerInterfaceType & other) const;
    bool operator!=(const NodeInterfaceType & other) const;
};


} //!avl

} //!tree

} //!format

} //!file

#endif //!FILE_FORMAT_TREE_AVL_POINTER_DEFN_H_
