#ifndef STRUC_TREE_AVL_INTF_POINTER_H_
#define STRUC_TREE_AVL_INTF_POINTER_H_

#include "Node.decl.h"

namespace struc
{
namespace tree
{
namespace avl
{
namespace intf
{

template<typename TData>
class Pointer
{
public:
    typedef TData DataType;
    typedef Node<DataType> NodeType;
public:
    Pointer() = default;
public:
    virtual ~Pointer() = default; 
public:
    Pointer(const Pointer & cpy) = default;
    Pointer(Pointer && mov) = default;
public:
    virtual Pointer<TData> & operator=(const Pointer<TData> & cpy) = 0;
    virtual Pointer<TData> & operator=(Pointer<TData> && mov) = 0;
public:
    virtual Pointer<TData> & Emplace(const DataType & data) = 0;
public:
    virtual Pointer<TData> & Displace() = 0;
public:
    virtual Pointer<TData> & operator=(const NodeType & other) = 0;
public:
    virtual NodeType & operator*() = 0;
    virtual const NodeType & operator*() const = 0;
public:
    virtual NodeType * operator->() = 0;
    virtual const NodeType * operator->() const = 0;
public:
    virtual operator bool() const = 0;
public:
    virtual bool operator==(const Pointer<TData> & other) const = 0;
    virtual bool operator==(const NodeType & other) const = 0;
    virtual bool operator!=(const Pointer<TData> & other) const = 0;
    virtual bool operator!=(const NodeType & other) const = 0;
};

} //!intf

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_INTF_POINTER_H_