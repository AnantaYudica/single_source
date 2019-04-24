#ifndef STRUC_TREE_AVL_INTF_NODE_H_
#define STRUC_TREE_AVL_INTF_NODE_H_

#include "Node.decl.h"
#include "Edge.h"

#include <cstdint>

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
    virtual bool HasParent() const = 0;
public:
    virtual Edge<TData> & Parent() = 0;
    virtual const Edge<TData> & Parent() const = 0;
public:
    virtual bool HasLeft() const = 0;
public:
    virtual Edge<TData> & Left() = 0;
    virtual const Edge<TData> & Left() const = 0;
public:
    virtual bool HasRight() const = 0;
public:
    virtual Edge<TData> & Right() = 0;
    virtual const Edge<TData> & Right() const = 0;
public:
    virtual HightValueType Hight() const = 0;
    virtual void Hight(const HightValueType & set) = 0;
public:
    virtual int Balance() const = 0;
public:
    virtual TData Data() const = 0;
    virtual void Data(const TData & data) = 0;
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