#ifndef STRUC_TREE_AVL_IMP_FILE_EDGE_H_
#define STRUC_TREE_AVL_IMP_FILE_EDGE_H_

#include "Edge.defn.h"
#include "Node.defn.h"

#include <fstream>

namespace struc
{
namespace tree
{
namespace avl
{
namespace imp
{
namespace file
{

template<typename TData>
void Edge<TData>::SetPosition(NodeType * point, const WayType & way,
    const PositionType & pos)
{
    if (!point || way == WayType::undefined) return;
    if (way == WayType::parent)
        point->ParentPosition(pos);
    else if (way == WayType::left)
        point->LeftPosition(pos);
    else if (way == WayType::right)
        point->RightPosition(pos);
}

template<typename TData>
typename Edge<TData>::PositionType 
Edge<TData>::GetPosition(NodeType const* point, const WayType & way)
{
    if (!point || way == WayType::undefined) return -1;
    if (way == WayType::parent)
        return point->ParentPosition();
    else if (way == WayType::left)
        return point->LeftPosition();
    else if (way == WayType::right)
        return point->RightPosition();
    return -1;
}

template<typename TData>
Edge<TData>::Edge() :
    m_point(),
    m_next(),
    m_way(WayType::undefined)
{}

template<typename TData>
Edge<TData>::Edge(const NodeType & point, WayType way) :
    m_point(point),
    m_next(std::move(m_point.Instance(GetPosition(&m_point, way)))),
    m_way(way)
{}

template<typename TData>
Edge<TData>::~Edge()
{}

template<typename TData>
Edge<TData>::Edge(const Edge & cpy) :
    m_point(cpy.m_point),
    m_next(cpy.m_next),
    m_way(cpy.m_way)
{}

template<typename TData>
Edge<TData>::Edge(Edge && mov) :
    m_point(std::move(mov.m_point)),
    m_next(std::move(mov.m_next)),
    m_way(mov.m_way)
{
    mov.Default();
}

template<typename TData>
Edge<TData> & Edge<TData>::operator=(const EdgeInterfaceType & cpy)
{
    const Edge<TData> * cast_cpy = dynamic_cast<Edge<TData> const*>(&cpy);
    if (!cast_cpy) return *this;
    m_point = cast_cpy->m_point;
    m_next = cast_cpy->m_next;
    m_way = cast_cpy->m_way;
    return *this;
}

template<typename TData>
Edge<TData> & Edge<TData>::operator=(EdgeInterfaceType && mov)
{
    Edge<TData> * cast_mov = dynamic_cast<Edge<TData> *>(&mov);
    if (!cast_mov) return *this;
    m_point = std::move(cast_mov->m_point);
    m_next = std::move(cast_mov->m_next);
    m_way = cast_mov->m_way;
    cast_mov->Default();
    return *this;
}

template<typename TData>
Edge<TData> & Edge<TData>::operator=(const NodeInterfaceType & node)
{
    const NodeType * cast_node = dynamic_cast<NodeType const*>(&node);
    if (!cast_node) return *this;
    SetPosition(&m_point, way, cast_node->Position());
    m_next = *cast_node;
    return *this;
}

template<typename TData>
Edge<TData> & Edge<TData>::operator=(NodeInterfaceType && node)
{
    NodeType * cast_node = dynamic_cast<NodeType *>(&node);
    if (!cast_node) return *this;
    SetPosition(&m_point, way, cast_node->Position());
    m_next = std::move(*cast_node);
    cast_node->Default();
    return *this;
}

template<typename TData>
void Edge<TData>::Default()
{
    m_way = WayType::undefined;
}

template<typename TData>
bool Edge<TData>::IsNull() const
{
    return  m_way == WayType::undefined ||
        !m_point || GetPosition(&m_point, m_way) == -1 ||
        m_point.IsDeleted();
}

template<typename TData>
typename Edge<TData>::PositionType Edge<TData>::Position() const
{
    return GetPosition(&m_point, way);
}

template<typename TData>
void Edge<TData>::Position(const PositionType & pos)
{
    SetPosition(&m_point, way, pos);
}

template<typename TData>
void Edge<TData>::Synchronize(const bool & force)
{
    m_point.Synchronize(force);
}

template<typename TData>
void Edge<TData>::Synchronize(const bool & force) const
{
    m_point.Synchronize(force);
}

template<typename TData>
typename Edge<TData>::NodeInterfaceType & Edge<TData>::Get()
{
    return m_point;
}

template<typename TData>
const typename Edge<TData>::NodeInterfaceType & Edge<TData>::Get() const
{
    return m_point;
}
    
template<typename TData>
typename Edge<TData>::NodeInterfaceType & Edge<TData>::operator*()
{
    return m_next;
}

template<typename TData>
const typename Edge<TData>::NodeInterfaceType & 
Edge<TData>::operator*() const
{
    return m_next;
}

template<typename TData>
typename Edge<TData>::NodeInterfaceType * Edge<TData>::operator->()
{
    return &m_next;
}

template<typename TData>
const typename Edge<TData>::NodeInterfaceType *
Edge<TData>::operator->() const
{
    return &m_next;
}

template<typename TData>
Edge<TData>::operator bool() const
{
    return !IsNull();
}

template<typename TData>
bool Edge<TData>::operator==(const EdgeInterfaceType & other) const
{
    auto cast_edge = dynamic_cast<Edge<TData> const*>(&other);
    return cast_edge && m_way == cast_edge->m_way && 
        m_point == cast_edge->m_point;
}

template<typename TData>
bool Edge<TData>::operator==(const NodeInterfaceType & other) const
{
    return !IsNull() && m_next == other;
}

template<typename TData>
bool Edge<TData>::operator!=(const EdgeInterfaceType & other) const
{
    auto cast_edge = dynamic_cast<Edge<TData> const*>(&other);
    return !cast_edge || m_way != cast_edge->m_way || 
        m_point != cast_edge->m_point;
}

template<typename TData>
bool Edge<TData>::operator!=(const NodeInterfaceType & other) const
{
    return IsNull() || m_next != other;
}

} //file

} //!imp

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_IMP_FILE_EDGE_H_
