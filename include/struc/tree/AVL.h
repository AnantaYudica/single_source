#ifndef STRUC_TREE_AVL_H_
#define STRUC_TREE_AVL_H_

#include "struc/tree/avl/intf/Node.h"
#include "struc/tree/avl/intf/Pointer.h"

#include <cstdint>
#include <stack>
#include <algorithm>
#include <memory>

namespace struc
{
namespace tree
{

class AVL
{
public:
    template<typename TData>
    using NodePointerType = std::shared_ptr<avl::intf::Node<TData>>;
public:
    AVL() = default;
private:
    template<typename TData>
    NodePointerType<TData> RotateRight(NodePointerType<TData> root);
    template<typename TData>
    NodePointerType<TData> RotateLeft(NodePointerType<TData> root);
private:
    template<typename TData>
    NodePointerType<TData> Rebalance(NodePointerType<TData> root);
public:
    template<typename TData, typename TCompareFunc>
    NodePointerType<TData> Find(NodePointerType<TData> root, 
        const TData & data, TCompareFunc compare_func);
    template<typename TData, typename TCompareFunc>
    NodePointerType<TData> Insert(NodePointerType<TData> root, 
        TData & data, TCompareFunc compare_func);
private:
    template<typename TData>
    NodePointerType<TData> Erase(NodePointerType<TData> root);
    template<typename TData>
    NodePointerType<TData> EraseMaximum(NodePointerType<TData> root, 
        NodePointerType<TData> swap);
    template<typename TData>
    NodePointerType<TData> EraseMinimum( NodePointerType<TData> root, 
         NodePointerType<TData> swap);
public:
    template<typename TData, typename TCompareFunc>
    NodePointerType<TData> Erase(NodePointerType<TData> root, 
        const TData & data, TCompareFunc compare_func);
};

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_H_