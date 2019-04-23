#ifndef STRUC_TREE_AVL_INTF_EDGE_H_
#define STRUC_TREE_AVL_INTF_EDGE_H_

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
class Edge
{
public:
    typedef TData DataType;
    typedef Node<DataType> NodeType;
public:
    Edge() = default;
public:
    virtual ~Edge() = default; 
public:
    Edge(const Edge<TData> & cpy) = default;
    Edge(Edge<TData> && mov) = default;
public:
    virtual Edge<TData> & operator=(const Edge<TData> & cpy) = 0;
    virtual Edge<TData> & operator=(Edge<TData> && mov) = 0;
public:
    virtual Edge<TData> & operator=(const NodeType & node) = 0;
    virtual Edge<TData> & operator=(NodeType && node) = 0;
public:
    virtual NodeType & Get() = 0;
    virtual const NodeType & Get() const = 0;
public:
    virtual NodeType & operator*() = 0;
    virtual const NodeType & operator*() const = 0;
public:
    virtual NodeType * operator->() = 0;
    virtual const NodeType * operator->() const = 0;
public:
    virtual operator bool() const = 0;
public:
    virtual bool operator==(const Edge<TData> & other) const = 0;
    virtual bool operator==(const NodeType & other) const = 0;
    virtual bool operator!=(const Edge<TData> & other) const = 0;
    virtual bool operator!=(const NodeType & other) const = 0;
};

} //!intf

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_INTF_EDGE_H_