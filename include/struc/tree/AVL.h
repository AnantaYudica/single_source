#ifndef STRUC_TREE_AVL_H_
#define STRUC_TREE_AVL_H_

#include "struc/tree/avl/intf/Node.h"
#include "struc/tree/avl/intf/Edge.h"

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
    using NodeType = avl::intf::Node<TData>;
public:
    AVL() = default;
private:
    template<typename TTrait, typename TData>
    void RotateRight(NodeType<TData> & root_node);
private:
    template<typename TTrait, typename TData>
    void RotateLeft(NodeType<TData> & root_node);
private:
    template<typename TTrait, typename TData>
    void Rebalance(NodeType<TData> & root_node);
public:
    template<typename TData, typename TCompareFunc>
    TData Find(NodeType<TData> & root_node, const TData & data, 
        TCompareFunc compare_func);
public:
    template<typename TTrait, typename TData, typename TCompareFunc>
    void Insert(NodeType<TData> & root_node, TData & data, 
        TCompareFunc compare_func);
private:
    template<typename TTrait, typename TData>
    void EraseMaximum(NodeType<TData> & root_node, 
        NodeType<TData> & swap_node);
private:
    template<typename TTrait, typename TData>
    void EraseMinimum(NodeType<TData> & root_node, 
        NodeType<TData> & swap_node);
private:
    template<typename TTrait, typename TData>
    TData Erase(NodeType<TData> & root_node);
public:
    template<typename TTrait, typename TData, typename TCompareFunc>
    TData Erase(NodeType<TData> & root_node, const TData & data, 
        TCompareFunc compare_func);
};

template<typename TTrait, typename TData>
void AVL::RotateRight(NodeType<TData> & root_node)
{
    if (!root_node) return;

    auto root_left_edge = TTrait::Assign(root_node.Left());
    auto root_left_right_edge = TTrait::Assign(root_left_edge->Right());

    TTrait::Assign(root_left_right_edge->Parent(), root_node);
    TTrait::Assign(root_node.Left(), *root_left_right_edge);
    
    TTrait::Assign(root_left_edge->Parent(), root_node.Parent());

    TTrait::Assign(root_node.Right(), root_node);
    TTrait::Assign(root_node.Parent(), *root_left_edge);

    TTrait::Hight(root_node, TTrait::Hight(root_node) + 1);
    TTrait::Hight(root_left_edge, TTrait::Hight(root_left_edge) + 1);
    
    TTrait::Assign(root_node, *root_left_edge);
}

template<typename TTrait, typename TData>
void AVL::RotateLeft(NodeType<TData> & root_node)
{
    if (!root_node) return;

    auto root_right_edge = TTrait::Assign(root_node.Right());
    auto root_right_left_edge = TTrait::Assign(root_right_edge->Left());

    TTrait::Assign(root_right_left_edge->Parent(), root_node);
    TTrait::Assign(root_node.Right(), *root_right_left_edge);

    TTrait::Assign(root_right_edge->Parent(), root_node.Parent());
  
    TTrait::Assign(root_right_edge->Left(), root_node);
    TTrait::Assign(root_node.Parent(), *root_right_edge);

    TTrait::Hight(root_node, TTrait::Hight(root_node) + 1);
    TTrait::Hight(root_right_edge, TTrait::Hight(root_right_edge) + 1);

    TTrait::Assign(root_node, *root_right_edge);
}

template<typename TTrait, typename TData>
void AVL::Rebalance(NodeType<TData> & root_node)
{
    if (!root_node) return;
    const auto balance = root_node.Balance();
    if (balance < -1)
    {
        const auto left_balance = root_node.Left()->Balance();
        if(left_balance <= 0)  
            RotateRight<TTrait>(root_node);
        else
        {
            auto left_node = TTrait::Assign(*(root_node.Left()));
            RotateLeft<TTrait>(left_node);
            TTrait::Assign(root_node.Left(), left_node);
            RotateRight<TTrait>(root_node);
        }
    }
    else if(balance > 1)
    {
        const auto right_balance = root_node.Right()->Balance();
        if (right_balance >= 0)
            return RotateLeft<TTrait>(root_node);
        else
        {
            auto right_node = TTrait::Assign(*(root_node.Right()));
            RotateRight<TTrait>(right_node);
            TTrait::Assign(root_node.Right(), right_node);
            RotateLeft<TTrait>(root_node);
        }
    }
}

template<typename TData, typename TCompareFunc>
TData AVL::Find(NodeType<TData> & root_node, const TData & data, 
    TCompareFunc compare_func)
{
    if (!root_node) return TData();
    const auto res = compare_func(root_node.Data(), data);
    if (res == 0)
        return root_node.Data();
    else if (res >= 1)
    {
        if (root_node.HasRight())
            return Find(*(root_node.Right()), data, compare_func);
        else
            return TData();
    }
    if (root_node.HasLeft())
        return Find(*(root_node.Left()), data, compare_func);
    return TData();
}

template<typename TTrait, typename TData, typename TCompareFunc>
void AVL::Insert(NodeType<TData> & root_node, TData & data, 
    TCompareFunc compare_func)
{
    if (!root_node) TTrait::Emplace(root_node, data);
    const auto res = compare_func(root_node.Data(), data);
    if (res == 0) return;
    else if (res >= 1)
    {
        if (root_node.HasRight())
        {
            auto node = TTrait::Assign(*(root_node.Right()));
            Insert<TTrait>(node, data, compare_func);
            TTrait::Assign(root_node.Right(), node);
        }
        else TTrait::Emplace(root_node.Right(), data);
    }
    else
    {
        if (root_node.HasLeft())
        {
            auto node = TTrait::Assign(*(root_node.Left()));
            Insert<TTrait>(node, data, compare_func);
            TTrait::Assign(root_node.Left(), node);
        }
        else TTrait::Emplace(root_node.Left(), data);
    }
    TTrait::Hight(root_node, TTrait::Hight(root_node) + 1);
    Rebalance<TTrait>(root_node);
}

template<typename TTrait, typename TData>
void AVL::EraseMaximum(NodeType<TData> & root_node, NodeType<TData> & swap_node)
{
    if (!root_node || !swap_node) return;
    if (swap_node.HasRight())
    {
        auto swap_right_node = TTrait::Assign(*(swap_node.Right()));
        EraseMaximum<TTrait>(root_node, swap_right_node);
        TTrait::Assign(swap_node.Right(), swap_right_node);
    }
    else
    {
        TTrait::Swap(root_node, swap_node);
        TTrait::Displace(swap_node);
        return;
    }
    
    TTrait::Hight(swap_node, TTrait::Hight(swap_node) + 1);
    Rebalance<TTrait>(swap_node);
}

template<typename TTrait, typename TData>
void AVL::EraseMinimum(NodeType<TData> & root_node, 
    NodeType<TData> & swap_node)
{
    if (!root_node || !swap_node) return;
    if (swap_node.HasLeft())
    {
        auto swap_left_node = TTrait::Assign(*(swap_node.Left()));
        EraseMinimum<TTrait>(root_node, swap_left_node);
        TTrait::Assign(swap_node.Left(), swap_left_node);
    }
    else
    {
        TTrait::Swap(root_node, swap_node);
        TTrait::Displace(swap_node);
        return;
    }
    
    TTrait::Hight(swap_node, TTrait::Hight(swap_node) + 1);
    Rebalance<TTrait>(swap_node);
}

template<typename TTrait, typename TData>
TData AVL::Erase(NodeType<TData> & root_node)
{
    if (!root_node) return TData();
    TData data = root_node.Data();
    if (!root_node.HasLeft() && root_node.HasRight())
    {
        auto node = TTrait::Assign(root_node);
        TTrait::Assign(root_node.Right()->Parent(), root_node.Parent());
        TTrait::Assign(root_node, *(root_node.Right()));
        TTrait::Displace(node);
    }
    else if (root_node.HasLeft() && !root_node.HasRight())
    {
        auto node = TTrait::Assign(root_node);
        TTrait::Assign(root_node.Left()->Parent(), root_node.Parent());
        TTrait::Assign(root_node, *(root_node.Left()));
        TTrait::Displace(node);
    }
    else if (root_node.HasLeft() && root_node.HasRight())
    {
        if (TTrait::Hight(root_node.Right()) > 
            TTrait::Hight(root_node.Left()))
        {
            auto left_node = TTrait::Assign(*(root_node.Left()));
            EraseMaximum<TTrait>(root_node, left_node);
        }
        else
        {
            auto right_node = TTrait::Assign(*(root_node.Right()));
            EraseMinimum<TTrait>(root_node, right_node);
        }
    }
    else TTrait::Displace(root_node);
    return data;
}

template<typename TTrait, typename TData, typename TCompareFunc>
TData AVL::Erase(NodeType<TData> & root_node, const TData & data, 
    TCompareFunc compare_func)
{
    if (!root_node) return TData();
    const auto res = compare_func(root_node.Data(), data);
    TData ret_data;
    if (res == 0)
        ret_data = Erase<TTrait>(root_node);
    else if (res == 1)
    {
        auto right_node = TTrait::Assign(*(root_node.Right()));
        ret_data = Erase<TTrait>(right_node, data, compare_func);
        TTrait::Assign(root_node.Right(), right_node);
    }
    else
    {
        auto left_node = TTrait::Assign(*(root_node.Left()));
        ret_data = Erase<TTrait>(left_node, data, compare_func);
        TTrait::Assign(root_node.Left(), left_node);
    }
   
    if (root_node)
    {
        TTrait::Hight(root_node, TTrait::Hight(root_node) + 1);
        Rebalance<TTrait>(root_node);
    }
    return ret_data;
}

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_H_