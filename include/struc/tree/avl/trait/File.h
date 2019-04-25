#ifndef STRUC_TREE_AVL_TRAIT_FILE_H_
#define STRUC_TREE_AVL_TRAIT_FILE_H_

#include "../intf/Edge.h"
#include "../intf/Node.h"
#include "../imp/file/Edge.h"
#include "../imp/file/Node.h"

#include <algorithm>

namespace struc
{
namespace tree
{
namespace avl
{
namespace trait
{

struct File
{
    File() = delete;
public:
    template<typename TData>
    static imp::file::Node<TData> Assign(const intf::Node<TData> & node);

    template<typename TData>
    static imp::file::Edge<TData> Assign(const intf::Edge<TData> & edge);
    template<typename TData>
    static void Assign(intf::Edge<TData> & edge, intf::Node<TData> & node);
    template<typename TData>
    static void Assign(intf::Edge<TData> & a_edge, intf::Edge<TData> & b_edge);
    template<typename TData>
    static void Assign(intf::Node<TData> & a_node, intf::Node<TData> & b_node);
public:
    template<typename TData>
    static void Emplace(intf::Edge<TData> & edge, const TData & data);
    template<typename TData>
    static void Emplace(intf::Node<TData> & node, const TData & data);
public:
    template<typename TData>
    static void Displace(intf::Edge<TData> & edge);
    template<typename TData>
    static void Displace(intf::Node<TData> & node);
public:
    template<typename TData>
    static typename imp::file::Node<TData>::HightValueType 
        Hight(intf::Edge<TData> & edge);
    template<typename TData>
    static typename imp::file::Node<TData>::HightValueType 
        Hight(intf::Node<TData> & node);
    template<typename TData>
    static void Hight(intf::Edge<TData> & edge,
        const typename imp::file::Node<TData>::HightValueType & set);
    template<typename TData>
    static void Hight(intf::Node<TData> & node,
        const typename imp::file::Node<TData>::HightValueType & set);
public:
    template<typename TData>
    static void Swap(intf::Node<TData> & a_node, intf::Node<TData> & b_node);
};

template<typename TData>
imp::file::Node<TData> File::Assign(const intf::Node<TData> & node)
{
    auto cast_node = dynamic_cast<imp::file::Node<TData> const*>(&node);
    if (!cast_node) return imp::file::Node<TData>();
    return imp::file::Node<TData>(*cast_node);
}

template<typename TData>
imp::file::Edge<TData> File::Assign(const intf::Edge<TData> & edge)
{
    auto cast_edge = dynamic_cast<imp::file::Edge<TData> const*>(&edge);
    if (!cast_edge) return imp::file::Edge<TData>();
    return imp::file::Edge<TData>(*cast_edge);
}

template<typename TData>
void File::Assign(intf::Edge<TData> & edge, intf::Node<TData> & node)
{
    auto cast_edge = dynamic_cast<imp::file::Edge<TData> *>(&edge);
    auto cast_node = dynamic_cast<imp::file::Node<TData> *>(&node);
    if (!cast_edge && !cast_node) return;
    cast_node->ParentPosition(cast_edge->Position());
    cast_node->Synchronize(false);
    cast_edge->Position(cast_node->Position());
    cast_edge->Synchronize(false);
}

template<typename TData>
void File::Assign(intf::Edge<TData> & a_edge, intf::Edge<TData> & b_edge)
{
    auto cast_a_edge = dynamic_cast<imp::file::Edge<TData> *>(&a_edge);
    auto cast_b_edge = dynamic_cast<imp::file::Edge<TData> *>(&b_edge);
    if (!cast_a_edge && !cast_b_edge) return;
    cast_a_edge->Position(cast_b_edge->Position());
    cast_a_edge->Synchronize(false);
}

template<typename TData>
void File::Assign(intf::Node<TData> & a_node, intf::Node<TData> & b_node)
{
    auto cast_a_node = dynamic_cast<imp::file::Node<TData> *>(&a_node);
    auto cast_b_node = dynamic_cast<imp::file::Node<TData> *>(&b_node);
    if (!cast_a_node && !cast_b_node) return;
    *cast_a_node = *cast_b_node;
}

template<typename TData>
void File::Emplace(intf::Edge<TData> & edge, const TData & data)
{
    auto cast_edge = dynamic_cast<imp::file::Edge<TData> *>(&edge);
    if (!cast_edge) return;
    auto cast_node = dynamic_cast<imp::file::Node<TData> *>(&(cast_edge->Get()));
    if (!cast_node) return;
    auto pos = cast_node->Allocate();
    if (pos == -1) return;
    imp::file::Node<TData> node = cast_node->Instance(pos);
    node.Data(data);
    node.ParentPosition(cast_node->Position());
    node.Synchronize(false);
    cast_edge->Position(pos);
    cast_edge->Synchronize(false);
}

template<typename TData>
void File::Emplace(intf::Node<TData> & node, const TData & data)
{
    auto cast_node = dynamic_cast<imp::file::Node<TData> *>(&node);
    if (!cast_node) return;
    auto pos = cast_node->Allocate();
    if (pos == -1) return;
    *cast_node = std::move(cast_node->Instance(pos));
    cast_node->Data(data);
    cast_node->Synchronize(false);
}

template<typename TData>
void File::Displace(intf::Edge<TData> & edge)
{
    auto cast_edge = dynamic_cast<imp::file::Edge<TData> *>(&edge);
    if (!cast_edge) return;
    Displace(edge);
    cast_edge.Position(-1);
    cast_edge.Synchronize(false);
}

template<typename TData>
void File::Displace(intf::Node<TData> & node)
{
    auto cast_node = dynamic_cast<imp::file::Node<TData> *>(&node);
    if (!cast_node) return;
    cast_node->ParentPosition(-1);
    cast_node->LeftPosition(-1);
    cast_node->RightPosition(-1);
    cast_node->Delete();
    cast_node->Synchronize(false);
    *cast_node = std::move(cast_node->Instance(-1));
}

template<typename TData>
typename imp::file::Node<TData>::HightValueType 
    File::Hight(intf::Edge<TData> & edge)
{
    return Hight(*edge);
}

template<typename TData>
typename imp::file::Node<TData>::HightValueType 
    File::Hight(intf::Node<TData> & node)
{
    auto cast_node = dynamic_cast<imp::file::Node<TData> *>(&node);
    if (!cast_node) return 0;
    typename imp::file::Node<TData>::HightValueType left = 0, right = 0;
    if (cast_node->HasLeft())
        left = cast_node->Left()->Hight();
    if (cast_node->HasRight())
        right = cast_node->Right()->Hight();
    return std::max(left, right);
}
template<typename TData>
void File::Hight(intf::Edge<TData> & edge,
    const typename imp::file::Node<TData>::HightValueType & set)
{
    return Hight(*edge, set);
}

template<typename TData>
void File::Hight(intf::Node<TData> & node,
    const typename imp::file::Node<TData>::HightValueType & set)
{
    auto cast_node = dynamic_cast<imp::file::Node<TData> *>(&node);
    if (!cast_node) return;
    cast_node->Hight(set);
    cast_node->Synchronize(false);
}

template<typename TData>
void File::Swap(intf::Node<TData> & a_node, intf::Node<TData> & b_node)
{
    auto cast_a_node = dynamic_cast<imp::file::Node<TData> *>(&a_node);
    auto cast_b_node = dynamic_cast<imp::file::Node<TData> *>(&b_node);
    if (!cast_a_node && !cast_b_node) return;
    auto tmp = cast_a_node->Data();
    cast_a_node->Data(cast_b_node->Data());
    cast_a_node->Synchronize(false);
    cast_b_node->Data(tmp);
    cast_b_node->Synchronize(false);
}

} //!trait

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_TRAIT_FILE_H_
