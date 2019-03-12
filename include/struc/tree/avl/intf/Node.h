#ifndef STRUC_TREE_AVL_INTF_NODE_H_
#define STRUC_TREE_AVL_INTF_NODE_H_

#include "Pointer.h"

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
    typede TData DataType;
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
    virtual Node<TData> & Emplace(TData & data) = 0;
public:
    virtual Node<TData> & Displace() = 0;
public:
    virtual Pointer<Node<TData>> & Parent() = 0;
    virtual Pointer<Node<TData>> & Right() = 0;
    virtual Pointer<Node<TData>> & Left() = 0;
public:
    virtual int Hight() = 0;
    virtual int Hight(int & set) = 0;
    virtual int Balance() = 0;
    virtual TData Value() = 0;
    virtual void Swap(Node<TData> & node) = 0;
public:
    virtual bool operator==(Node<TData> other) const = 0;
    virtual bool operator!=(Node<TData> other) const = 0;
public:
    virtual operator bool() = 0;
};

} //!intf

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_INTF_NODE_H_