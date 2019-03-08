#ifndef STRUC_TREE_AVL_H_
#define STRUC_TREE_AVL_H_

#include "struc/tree/avl/intf/Node.h"
#include "struc/tree/avl/intf/Pointer.h"

#include <cstdint>

namespace struc
{
namespace tree
{

class AVL
{
public:
    AVL() = default;
private:
    template<typename TData>
    avl::intf::Node<TData> RotateRight(avl::intf::Node<TData> root);
    template<typename TData>
    avl::intf::Node<TData> RotateLeft(avl::intf::Node<TData> root);
private:
    template<typename TData>
    avl::intf::Node<TData> Rebalance(avl::intf::Node<TData> root);
public:
    template<typename TData, typename TCompareFunc>
    avl::intf::Node<TData> Find(avl::intf::Node<TData> root, 
        const TData & data, TCompareFunc compare_func);
    template<typename TData, typename TCompareFunc>
    avl::intf::Node<TData> Insert(avl::intf::Node<TData> root, 
        TData & data, TCompareFunc compare_func);
private:
    template<typename TData>
    avl::intf::Node<TData> Erase(avl::intf::Node<TData> root);
    template<typename TData>
    avl::intf::Node<TData> EraseMaximum(avl::intf::Node<TData> root, 
        avl::intf::Node<TData> swap);
    template<typename TData>
    avl::intf::Node<TData> EraseMinimum(avl::intf::Node<TData> root, 
        avl::intf::Node<TData> swap);
public:
    template<typename TData, typename TCompareFunc>
    avl::intf::Node<TData> Erase(avl::intf::Node<TData> root, 
        const TData & data, TCompareFunc compare_func);
};

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_H_