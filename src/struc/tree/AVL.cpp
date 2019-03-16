#include "struc/tree/AVL.h"

using namespace std;
using namespace struc::tree;

template<typename TData>
typename AVL::NodePointerType<TData> 
AVL::RotateRight(NodePointerType<TData> root)
{
    if (!root || !*root) return nullptr;
    auto & root_left = *(root->Left());
    auto & root_left_right = *(root_left.Right());

    root->Left() = root_left_right;
    if (root_left_right)
        root_left_right.Parent() = *root;
    
    root_left.Right() = *root;
    root_left.Parent() = *(root->Parent());
    root.Parent() = root_left;

    root.Hight(max(root->Left()->Hight(), root->Right()->Hight()) + 1);
    root_left.Hight(max(root_left.Left()->Hight(), 
        root_left.Right()->Hight()) + 1);
    
    return std::move(root_left.MakeMove());
}

template<typename TData>
typename AVL::NodePointerType<TData> 
AVL::RotateLeft(NodePointerType<TData> root)
{
    if (!root || !*root) return nullptr;
    auto & root_right = *(root->Right());
    auto & root_right_left = *(root_right.Left());

    root.Right() = root_right_left;
    if (root_right_left)
        root_right_left.Parent() = *root;
    
    root_right.Left() = *root;
    root_right.Parent() = *(root.Parent());
    root->Parent() = root_right;

    root.Hight(max(root->Left()->Hight(), root->Right()->Hight()) + 1);
    root_right.Hight(max(root_right.Left()->Hight(), 
        root_right.Right()->Hight()) + 1);

    return std::move(root_right.MakeMove());
}

template<typename TData>
typename AVL::NodePointerType<TData> 
AVL::Rebalance(NodePointerType<TData> root)
{
    if (!root || !*root) return nullptr;
    const auto right_balance = root.Right()->Balance();
    const auto left_balance = root.Left()->Balance();
    if (right_balance >= 0)
        return RotateLeft(root);
    else if(left_balance <= 0)  
        return RotateRight(root);
    else if(right_balance == -1)
    {
        return RotateLeft(RotateRight(root->Right()->MakeCopy()));
    }
    else if(left_balance == 1)
    {
        return RotateRight(RotateLeft(root->Left()->MakeCopy()));
    }
    return root;
}

template<typename TData, typename TCompareFunc>
typename AVL::NodePointerType<TData> 
AVL::Find(NodePointerType<TData> root, const TData & data, 
    TCompareFunc compare_func)
{
    if (!root || !*root) return nullptr;
    const auto res = compare_func(root->Value(), data);
    if (res == 0)
        return root;
    else if (res == 1)
        return Find(root->Right()->MakeCopy(), data, compare_func);
    return Find(root->Left()->MakeCopy(), data, compare_func);
}

template<typename TData, typename TCompareFunc>
typename AVL::NodePointerType<TData> AVL::Insert(NodePointerType<TData> root, 
    TData & data, TCompareFunc compare_func)
{
    if (!root) return nullptr;
    if (!*root)
    {
        root->Emplace(data);
        return root;
    }
    const auto res = compare_func(root->Value(), data);
    if (res == 0)
        return root;
    else if (res == 1)
    {
        if (*(root->Right()))
            Insert(root->Right()->MakeCopy(), data, compare_func);
        else
            root->Right().Emplace(data);
    }
    else
    {
        if (*(root->Left()))
            Insert(root->Left()->MakeCopy(), data, compare_func);
        else
            root->Left().Emplace(data);
    }

    root->Hight(max(root->Left()->Hight(), root->Right()->Hight()) + 1);
    return Rebalance(root);
}

template<typename TData>
typename AVL::NodePointerType<TData> AVL::Erase(NodePointerType<TData> root)
{
    const bool has_right = root->Right();
    const bool has_left = root->Left();
    if (!has_left && has_right)
    {
        if (root->Parent()->Right() == *root)
            root.Parent()->Right() = *(root->Right());
        else if (root->Parent()->Left() == *root)
            root->Parent()->Left() = *(root->Right());
        root->Right()->Parent() = *(root->Parent());
    }
    else if (has_left && !has_right)
    {
        if (root->Parent()->Right() == *root)
            root->Parent()->Right() = *(root->Left());
        else if (root->Parent()->Left() == *root)
            root->Parent()->Left() = *(root->Left());
        root->Left()->Parent() = *(root->Parent());
    }
    else if (has_left && has_right)
    {
        if (root->Parent()->Right()->Hight() >
            root->Parent()->Left()->Hight())
            EraseMaximum(root, root->Left()->MakeCopy());
        else
            EraseMinimum(root, root->Right()->MakeCopy());
    }
    else
    {
        if (root->Parent()->Right() == *root)
            root->Parent()->Right().Displace();
        else if (root->Parent()->Left() == root)
            root->Parent()->Left().Displace();
    }

    root->Parent().Displace();
    return root;
}

template<typename TData>
typename AVL::NodePointerType<TData> 
AVL::EraseMaximum(NodePointerType<TData> root, NodePointerType<TData> swap)
{
    if (swap->Right())
        EraseMaximum(root, swap->Right()->MakeCopy());
    else
    {
        root->Swap(*swap);
        Erase(swap);
        swap->Displace();
        return root;
    }
    
    root->Hight(max(root->Left()->Hight(), root->Right()->Hight()) + 1);
    return Rebalance(root);
        
}

template<typename TData>
typename AVL::NodePointerType<TData> 
AVL::EraseMinimum(NodePointerType<TData> root, NodePointerType<TData> swap)
{
    if (swap->Left())
        EraseMaximum(root, *swap->Left());
    else
    {
        root->Swap(*swap);
        Erase(swap);
        swap->Displace();
        return root;
    }
    
    root->Hight(max(root->Left()->Hight(), root->Right()->Hight()) + 1);
    return Rebalance(root);
}

template<typename TData, typename TCompareFunc>
typename AVL::NodePointerType<TData>  
AVL::Erase(NodePointerType<TData> root, const TData & data, 
    TCompareFunc compare_func)
{
    if (!root || !*root) return nullptr;
    const auto res = compare_func(root->Value(), data);
    if (res == 0)
    {
        Erase(root);
    }
    else if (res == 1)
    {
        if (!Erase(root->Right()->MakeCopy(), data, compare_func)) 
            return root;
    }
    else
    {
        if (!Erase(root->Left()->MakeCopy(), data, compare_func)) 
            return root;
    }
   
    root->Hight(max(root->Left()->Hight(), root->Right()->Hight()) + 1);
    return Rebalance(root);
}
