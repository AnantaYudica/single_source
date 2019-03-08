#ifndef STRUC_TREE_AVL_INTF_POINTER_H_
#define STRUC_TREE_AVL_INTF_POINTER_H_

namespace struc
{
namespace tree
{
namespace avl
{
namespace intf
{

template<typename TNode>
class Pointer
{
public:
    Pointer() = default;
public:
    virtual ~Pointer() = default; 
public:
    Pointer(const Pointer & cpy) = default;
    Pointer(Pointer && mov) = default;
public:
    virtual Pointer<TNode> & operator=(const Pointer & cpy) = 0;
    virtual Pointer<TNode> & operator=(Pointer && mov) = 0;
public:
    virtual TNode Emplace(typename TNode::DataType & data) = 0;
public:
    virtual TNode Displace() = 0;
public:
    virtual Pointer<TNode> & operator=(const Pointer<TNode> & node) = 0;
    virtual operator bool() = 0;
    virtual TNode operator*() = 0;
    virtual TNode operator->() = 0;
public:
    virtual bool operator==(Pointer<TNode> other) = 0;
    virtual bool operator==(TNode other) = 0;
    virtual bool operator!=(Pointer<TNode> other) = 0;
    virtual bool operator!=(TNode other) = 0;
};

} //!intf

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_INTF_POINTER_H_