#ifndef STRUC_TREE_AVL_IMP_FILE_EDGE_H_
#define STRUC_TREE_AVL_IMP_FILE_EDGE_H_

#include "Edge.defn.h"
#include "Node.defn.h"

#include <fstream>
#include <utility>

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
template<typename ... TArgs>
void Edge<TData>::CreateInstance(NodeType ** node, TArgs && ... args)
{
    if (!node || *node) return;
    *node = new NodeType(std::forward<TArgs>(args)...);
}

template<typename TData>
template<typename ... TArgs>
void Edge<TData>::CreateInstance(NodeType *const* node, 
    TArgs && ... args)
{
    auto cast_node = const_cast<NodeType **>(node);
    CreateInstance(cast_node, std::forward<TArgs>(args)...);
}

template<typename TData>
void Edge<TData>::Reset(NodeType ** node)
{
    if (!node || !*node) return;
    delete *node;
    *node = nullptr;
}

template<typename TData>
void Edge<TData>::Reset(NodeType *const* node)
{
    auto cast_node = const_cast<NodeType **>(node);
    Reset(cast_node);
}

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
    m_point(nullptr),
    m_next(nullptr),
    m_way(WayType::undefined)
{}

template<typename TData>
Edge<TData>::Edge(const NodeType & point, WayType way) :
    m_point(nullptr),
    m_next(nullptr),
    m_way(way)
{
    if (point) 
    {
        CreateInstance(&m_point, point);
        const auto pos = GetPosition(m_point, way);
        if (pos != -1) 
            CreateInstance(&m_next, std::move(m_point->Instance(pos)));
    }   
}

template<typename TData>
Edge<TData>::~Edge()
{
    Reset(&m_point),
    Reset(&m_next);
}

template<typename TData>
Edge<TData>::Edge(const Edge & cpy) :
    m_point(nullptr),
    m_next(nullptr),
    m_way(cpy.m_way)
{
    if (cpy.m_point) CreateInstance(&m_point, *(cpy.m_point));
    if (cpy.m_next) CreateInstance(&m_next, *(cpy.m_next));
}

template<typename TData>
Edge<TData>::Edge(Edge && mov) :
    m_point(std::move(mov.m_point)),
    m_next(std::move(mov.m_next)),
    m_way(mov.m_way)
{
    mov.m_way = WayType::undefined; 
}

template<typename TData>
Edge<TData> & Edge<TData>::operator=(const EdgeInterfaceType & cpy)
{
    const Edge<TData> * cast_cpy = dynamic_cast<Edge<TData> const*>(&cpy);
    if (!cast_cpy) return *this;
    Reset(&m_point);
    if (cast_cpy->m_point) CreateInstance(&m_point, *(cast_cpy->m_point));
    Reset(&m_next);
    if (cast_cpy->m_next) CreateInstance(&m_next, *(cast_cpy->m_next));
    m_way = cast_cpy->m_way;
    return *this;
}

template<typename TData>
Edge<TData> & Edge<TData>::operator=(EdgeInterfaceType && mov)
{
    Edge<TData> * cast_mov = dynamic_cast<Edge<TData> *>(&mov);
    if (!cast_mov) return *this;
    Reset(&m_point);
    m_point = std::move(cast_mov->m_point);
    cast_mov->m_point = nullptr;
    Reset(&m_next);
    m_next = std::move(cast_mov->m_next);
    cast_mov->m_next = nullptr;
    m_way = cast_mov->m_way;
    return *this;
}

template<typename TData>
Edge<TData> & Edge<TData>::operator=(const NodeInterfaceType & node)
{
    const NodeType * cast_node = dynamic_cast<NodeType const*>(&node);
    if (!cast_node || !m_point || !m_next) return *this;
    SetPosition(m_point, m_way, cast_node->Position());
    *m_next = *cast_node;
    return *this;
}

template<typename TData>
Edge<TData> & Edge<TData>::operator=(NodeInterfaceType && node)
{
    NodeType * cast_node = dynamic_cast<NodeType *>(&node);
    if (!cast_node || !m_point || !m_next) return *this;
    SetPosition(m_point, m_way, cast_node->Position());
    *m_next = std::move(*cast_node);
    m_next->ParentPosition(m_point->Position());
    m_next->Synchronize(false);
    return *this;
}

template<typename TData>
typename Edge<TData>::PositionType Edge<TData>::Position() const
{
    return GetPosition(m_point, m_way);
}

template<typename TData>
void Edge<TData>::Position(const PositionType & pos)
{
    if (!m_point) return;
    SetPosition(m_point, m_way, pos);
}

template<typename TData>
void Edge<TData>::Synchronize(const bool & force)
{
    if (!m_point) return;
    m_point->Synchronize(force);
    Update();
}

template<typename TData>
void Edge<TData>::Synchronize(const bool & force) const
{
    auto cast_edge = const_cast<Edge<TData> *>(this);
    cast_edge->Synchronize(force);
}

template<typename TData>
void Edge<TData>::Update()
{
    const auto pos = GetPosition(m_point, m_way);
    if (pos == -1) Reset(&m_next);
    else if (m_next && pos != m_next->Position())
        *m_next = std::move(m_point->Instance(pos));
    else if (!m_next && pos != -1)
        CreateInstance(&m_next, std::move(m_point->Instance(pos)));
}

template<typename TData>
typename Edge<TData>::NodeInterfaceType & Edge<TData>::Get()
{
    return *m_point;
}

template<typename TData>
const typename Edge<TData>::NodeInterfaceType & Edge<TData>::Get() const
{
    return *m_point;
}
    
template<typename TData>
typename Edge<TData>::NodeInterfaceType & Edge<TData>::operator*()
{
    return *m_next;
}

template<typename TData>
const typename Edge<TData>::NodeInterfaceType & 
Edge<TData>::operator*() const
{
    return *m_next;
}

template<typename TData>
typename Edge<TData>::NodeInterfaceType * Edge<TData>::operator->()
{
    return m_next;
}

template<typename TData>
const typename Edge<TData>::NodeInterfaceType *
Edge<TData>::operator->() const
{
    return m_next;
}

template<typename TData>
Edge<TData>::operator bool() const
{
    return m_way == WayType::undefined || !m_point ||
        !m_next || GetPosition(m_point, m_way) == -1 ||
        m_point->IsDeleted();
}

template<typename TData>
bool Edge<TData>::operator==(const EdgeInterfaceType & other) const
{
    auto cast_edge = dynamic_cast<Edge<TData> const*>(&other);
    if (!cast_edge) return false;
    if (m_point == cast_edge->m_point) return true;
    return m_way == cast_edge->m_way && 
        (m_point && cast_edge->m_point) &&
        *m_point == *(cast_edge->m_point)&&
        (m_next && cast_edge->m_next) &&
        *m_next == *(cast_edge->m_next);
}

template<typename TData>
bool Edge<TData>::operator==(const NodeInterfaceType & other) const
{
    auto cast_node = dynamic_cast<Node<TData> const*>(&other);
    if (!cast_node) return false;
    if ((!m_point || !m_next) && !*cast_node) return true;
    return m_next && *m_next == *cast_node;
}

template<typename TData>
bool Edge<TData>::operator!=(const EdgeInterfaceType & other) const
{
    return !(*this == other);
}

template<typename TData>
bool Edge<TData>::operator!=(const NodeInterfaceType & other) const
{
    return !(*this == other);
}

} //file

} //!imp

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_IMP_FILE_EDGE_H_
