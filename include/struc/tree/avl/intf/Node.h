#ifndef STRUC_TREE_AVL_INTF_NODE_H_
#define STRUC_TREE_AVL_INTF_NODE_H_

#include "Node.decl.h"
#include "Pointer.h"

#include <memory>

namespace struc
{
namespace tree
{
namespace avl
{
namespace intf
{

template<typename TData>
class Node
{
public:
    typedef std::uint32_t HightValueType;
    typedef TData DataType;
    typedef std::shared_ptr<Node<DataType>> PointerType;
protected:
    Node() = default;
public:
    virtual ~Node() = default; 
public:
    Node(const Node<TData> &) = default;
    Node(Node<TData> &&) = default;
public:
    virtual Node<TData> & operator=(const Node<TData> &) = 0;
    virtual Node<TData> & operator=(Node<TData> &&) = 0;
public:
    virtual Node<TData> & operator=(const TData & data) = 0;
public:
    virtual PointerType MakeCopy() const = 0;
    virtual PointerType MakeMove() = 0;
public:
    virtual Node<TData> & Emplace(const TData & data) = 0;
public:
    virtual Node<TData> & Displace() = 0;
public:
    virtual Pointer<TData> & Parent() = 0;
    virtual const Pointer<TData> & Parent() const = 0;
public:
    virtual Pointer<TData> & Right() = 0;
    virtual const Pointer<TData> & Right() const = 0;
public:
    virtual Pointer<TData> & Left() = 0;
    virtual const Pointer<TData> & Left() const = 0;
public:
    virtual int Hight() const = 0;
    virtual int Hight(const HightValueType & set) = 0;
public:
    virtual int Balance() = 0;
    virtual int Balance() const = 0;
public:
    virtual void Swap(Node<TData> & other) = 0;
public:
    virtual TData operator*() const = 0;
public:
    virtual bool operator==(const Node<TData> & other) const = 0;
    virtual bool operator!=(const Node<TData> & other) const = 0;
public:
    virtual operator bool() const = 0;
};

} //!intf

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_INTF_NODE_H_