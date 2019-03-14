#include "struc/tree/AVL.h"

#include <algorithm>

using namespace std;
using namespace struc::tree;

template<typename TData>
avl::intf::Node<TData> AVL::RotateRight(avl::intf::Node<TData> root)
{
    if (!root) return {};
    auto root_left = *(root.Left());
    auto root_left_right = *(root_left.Right());

    root.Left() = root_left_right;
    if (root_left_right)
        root_left_right.Parent() = root;
    
    root_left.Right() = root;
    root_left.Parent() = *(root.Parent());
    root.Parent() = root_left;

    root.Hight(max(root.Left()->Hight(), root.Right()->Hight()) + 1);
    root_left.Hight(max(root_left.Left()->Hight(), 
        root_left.Right()->Hight()) + 1);
    return root_left;
}

template<typename TData>
avl::intf::Node<TData> AVL::RotateLeft(avl::intf::Node<TData> root)
{
    if (!root) return {};
    auto root_right = *(root.Right());
    auto root_right_left = *(root_right.Left());

    root.Right() = root_right_left;
    if (root_right_left)
        root_right_left.Parent() = root;
    
    root_right.Left() = root;
    root_right.Parent() = *(root.Parent());
    root.Parent() = root_right;

    root.Hight(max(root.Left()->Hight(), root.Right()->Hight()) + 1);
    root_right.Hight(max(root_right.Left()->Hight(), 
        root_right.Right()->Hight()) + 1);
    return root_right;
}

template<typename TData>
avl::intf::Node<TData> AVL::Rebalance(avl::intf::Node<TData> root)
{
    if (!root) return{};
    const auto right_balance = root.Right()->Balance();
    const auto left_balance = root.Left()->Balance();
    if (right_balance >= 0)
        return RotateLeft(root);
    else if(left_balance <= 0)  
        return RotateRight(root);
    else if(right_balance == -1)
    {
        RotateRight(*root->Right());
        return RotateLeft(root);
    }
    else if(left_balance == 1)
    {
        RotateLeft(*root->Left());
        return RotateRight(root);
    }
    return root;
}

template<typename TData, typename TCompareFunc>
avl::intf::Node<TData> AVL::Find(avl::intf::Node<TData> root, 
    const TData & data, TCompareFunc compare_func)
{
    if (!root) return {};
    const auto res = compare_func(root.Value(), data);
    if (res == 0)
        return root;
    else if (res == 1)
        return Find(*root.Right(), data, compare_func);
    return Find(*root.Left(), data, compare_func);
}

template<typename TData, typename TCompareFunc>
avl::intf::Node<TData> AVL::Insert(avl::intf::Node<TData> root, 
    TData & data, TCompareFunc compare_func)
{
    if (!root) return root.Emplace(data);
    const auto res = compare_func(root.Value(), data);
    if (res == 0)
        return root;
    else if (res == 1)
    {
        if (*root.Right())
            Insert(*root.Right(), data, compare_func);
        root.Right().Emplace(data);
    }
    else
    {
        if (*root.Left())
            Insert(*root.Left(), data, compare_func);
        root.Left().Emplace(data);
    }

    root.Hight(max(root.Left()->Hight(), root.Right()->Hight()) + 1);
    return Rebalance(root);
}

template<typename TData>
avl::intf::Node<TData> AVL::EraseMaximum(avl::intf::Node<TData> root, 
    avl::intf::Node<TData> swap)
{
    if (swap->Right())
        EraseMaximum(root, *swap->Right());
    else
    {
        avl::intf::Node<TData> swap_node;
        root.Swap(swap);
        Erase(swap);
        swap.Displace();
        return root;
    }
    
    root.Hight(max(root.Left()->Hight(), root.Right()->Hight()) + 1);
    return Rebalance(root);
        
}

template<typename TData>
avl::intf::Node<TData> AVL::EraseMinimum(avl::intf::Node<TData> root, 
    avl::intf::Node<TData> swap)
{
    if (swap->Left())
        EraseMaximum(root, *swap->Left());
    else
    {
        avl::intf::Node<TData> swap_node;
        root.Swap(swap);
        Erase(swap);
        swap.Displace();
        return root;
    }
    
    root.Hight(max(root.Left()->Hight(), root.Right()->Hight()) + 1);
    return Rebalance(root);
}
    
template<typename TData>
avl::intf::Node<TData> AVL::Erase(avl::intf::Node<TData> root)
{
    const bool has_right = root.Right();
    const bool has_left = root.Left();
    if (!has_left && has_right)
    {
        if (root.Parent()->Right() == root)
            root.Parent()->Right() = *(root.Right());
        else if (root.Parent()->Left() == root)
            root.Parent()->Left() = *(root.Right());
        root.Right()->Parent() = *(root.Parent());
    }
    else if (has_left && !has_right)
    {
        if (root.Parent()->Right() == root)
            root.Parent()->Right() = *(root.Left());
        else if (root.Parent()->Left() == root)
            root.Parent()->Left() = *(root.Left());
        root.Left()->Parent() = *(root.Parent());
    }
    else if (has_left && has_right)
    {
        if (root.Parent()->Right()->Hight() >
            root.Parent()->Left()->Hight())
            EraseMaximum(root, *root->Left());
        else
            EraseMinimum(root, *root->Right());
    }
    else
    {
        if (root.Parent()->Right() == root)
            root.Parent()->Right().Displace();
        else if (root.Parent()->Left() == root)
            root.Parent()->Left().Displace();
    }

    root.Parent().Displace();
    return root;
}


template<typename TData, typename TCompareFunc>
avl::intf::Node<TData> AVL::Erase(avl::intf::Node<TData> root, 
    const TData & data, TCompareFunc compare_func)
{
    if (!root) return {};
    const auto res = compare_func(root.Value(), data);
    if (res == 0)
    {
        Erase(root);
    }
    else if (res == 1)
    {
        if (!Erase(*root.Right(), data, compare_func)) return root;
    }
    else
    {
        if (!Erase(*root.Left(), data, compare_func)) return root;
    }
   
    root.Hight(max(root.Left()->Hight(), root.Right()->Hight()) + 1);
    return Rebalance(root);
}
