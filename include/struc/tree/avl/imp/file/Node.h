#ifndef STRUC_TREE_AVL_IMP_FILE_NODE_H_
#define STRUC_TREE_AVL_IMP_FILE_NODE_H_

#include "Node.defn.h"
#include "Edge.defn.h"
#include "node/mgmt/Point.h"

#include "../../../../../intf/file/Input.h"
#include "../../../../../intf/file/Output.h"
#include "../../../../../defn/rec/Size.h"
#include "../../../../../defn/rec/Status.h"

#include <utility>
#include <cstring>

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
void Node<TData>::CreateInstance(Edge<TData> ** edge, TArgs && ... args)
{
    if (!edge || *edge) return;
    *edge = new Edge<TData>(std::forward<TArgs>(args)...);
}

template<typename TData>
template<typename ... TArgs>
void Node<TData>::CreateInstance(Edge<TData> *const* edge, TArgs && ... args)
{
    auto cast_edge = const_cast<Edge<TData> **>(edge);
    CreateInstance(cast_edge, std::forward<TArgs>(args)...);
}

template<typename TData>
template<typename ... TArgs>
void Node<TData>::CreateInstance(PointType ** point, TArgs && ... args)
{
    if (!point || *point) return;
    *point = node::mgmt::Point<TData>::GetInstance().
        Allocate(std::forward<TArgs>(args)...);
}

template<typename TData>
template<typename ... TArgs>
void Node<TData>::CreateInstance(PointType *const* point, TArgs && ... args)
{
    auto cast_point = const_cast<PointType **>(point);
    CreateInstance(cast_point, std::forward<TArgs>(args)...);
}

template<typename TData>
void Node<TData>::Reset(Edge<TData> ** edge)
{
    if (!edge || !*edge) return;
    delete *edge;
    *edge = nullptr;
}

template<typename TData>
void Node<TData>::Reset(Edge<TData> *const* edge)
{
    auto cast_edge = const_cast<Edge<TData> **>(edge);
    Reset(cast_edge);
}

template<typename TData>
void Node<TData>::Reset(PointType ** point)
{
    if (!point || !*point) return;
    node::mgmt::Point<TData>::GetInstance().Deallocate(*point);
    point = nullptr;
}

template<typename TData>
void Node<TData>::Reset(PointType *const* point)
{
    auto cast_point = const_cast<PointType **>(point);
    Reset(cast_point);
}

template<typename TData>
Node<TData>::Node() :
    m_point(nullptr),
    m_parent(nullptr),
    m_right(nullptr),
    m_left(nullptr),
    m_file(nullptr),
    m_fileFormatLinear(nullptr)
{}

template<typename TData>
Node<TData>::Node(FileInterfacePointerType file, 
    FileFormatLinearPointerType format) :
        m_point(nullptr),
        m_parent(nullptr),
        m_right(nullptr),
        m_left(nullptr),
        m_file(file),
        m_fileFormatLinear(format)
{}

template<typename TData>
Node<TData>::Node(FileInterfacePointerType file, 
    FileFormatLinearPointerType format, const PositionType & position) :
        m_point(nullptr),
        m_parent(nullptr),
        m_right(nullptr),
        m_left(nullptr),
        m_file(file),
        m_fileFormatLinear(format)
{
    if (position != -1 && IsValid()) 
    {
        CreateInstance(&m_point, position);
        m_fileFormatLinear->SeekPosition(position);
        m_fileFormatLinear->CurrentGet(m_point->Record());
        if (m_point->Record().Parent() != -1)
            CreateInstance(&m_parent, *this, EdgeWayType::parent);
        if (m_point->Record().Left() != -1)
            CreateInstance(&m_left, *this, EdgeWayType::left);
        if (m_point->Record().Right() != -1)
            CreateInstance(&m_right, *this, EdgeWayType::right);
    }
}

template<typename TData>
Node<TData>::~Node()
{
    Reset(&m_point);
    Reset(&m_parent);
    Reset(&m_right);
    Reset(&m_left);
}

template<typename TData>
Node<TData>::Node(const Node<TData> & cpy) :
    m_point(nullptr),
    m_parent(nullptr),
    m_right(nullptr),
    m_left(nullptr),
    m_file(cpy.m_file),
    m_fileFormatLinear(cpy.m_fileFormatLinear)
{
    if (cpy)
    {
        CreateInstance(&m_point, cpy.m_point);
        if (cpy.HasParent())
            CreateInstance(&m_parent, *this, EdgeWayType::parent);
        if (cpy.HasLeft())
            CreateInstance(&m_left, *this, EdgeWayType::left);
        if (cpy.HasRight())
            CreateInstance(&m_right, *this, EdgeWayType::right);
    }
}

template<typename TData>
Node<TData>::Node(Node<TData> && mov) :
    m_point(std::move(mov.m_point)),
    m_parent(std::move(mov.m_parent)),
    m_right(std::move(mov.m_right)),
    m_left(std::move(mov.m_left)),
    m_file(std::move(mov.m_file)),
    m_fileFormatLinear(std::move(mov.m_fileFormatLinear))
{
    mov.m_point = nullptr;
    mov.m_parent = nullptr;
    mov.m_right = nullptr;
    mov.m_left = nullptr;
    mov.m_file = nullptr;
    mov.m_fileFormatLinear = nullptr;
}

template<typename TData>
Node<TData> & Node<TData>::operator=(const Node<TData> & cpy)
{
    Reset(&m_point);
    CreateInstance(&m_point, cpy.m_point);
    Reset(&m_parent);
    if (cpy.HasParent())
        CreateInstance(&m_parent, *this, EdgeWayType::parent);
    Reset(&m_left);
    if (cpy.HasLeft())
        CreateInstance(&m_left, *this, EdgeWayType::left);
    Reset(&m_right);
    if (cpy.HasRight())
        CreateInstance(&m_right, *this, EdgeWayType::right);
    m_file = cpy.m_file;
    m_fileFormatLinear = cpy.m_fileFormatLinear;
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::operator=(Node<TData> && mov)
{
    Reset(&m_point);
    m_point = std::move(mov.m_point);
    mov.m_point = nullptr;
    Reset(&m_parent);
    m_parent = std::move(mov.m_parent);
    mov.m_parent = nullptr;
    Reset(&m_right);
    m_right = std::move(mov.m_right);
    mov.m_right = nullptr;
    Reset(&m_left);
    m_left = std::move(mov.m_left);
    mov.m_left = nullptr;
    m_file = std::move(mov.m_file);
    mov.m_file = nullptr;
    m_fileFormatLinear = std::move(mov.m_fileFormatLinear);
    mov.m_fileFormatLinear = nullptr;
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::operator=(const NodeInterfaceType & cpy)
{
    auto cast_cpy = dynamic_cast<Node<TData> const*>(&cpy);
    if (cast_cpy) *this = *cast_cpy;
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::operator=(NodeInterfaceType && mov)
{
    auto cast_mov = dynamic_cast<Node<TData> *>(&mov);
    if (cast_mov) *this = std::move(*cast_mov);
    return *this;
}

template<typename TData>
bool Node<TData>::IsValid() const
{
    return m_file && m_file->IsOpen() && m_fileFormatLinear &&
        m_file == m_fileFormatLinear->File();
}

template<typename TData>
Node<TData> Node<TData>::Instance(PositionType position) const
{
    return Node<TData>{m_file, m_fileFormatLinear, position};
}

template<typename TData>
typename Node<TData>::PositionType Node<TData>::Allocate()
{
    if (!IsValid()) return -1;
    PositionType position = m_fileFormatLinear->Size();
        static_cast<int>(::defn::rec::Size<RecordType>::Value);
    m_fileFormatLinear->SeekOffset(0, WayType::end);
    m_fileFormatLinear->CurrentPut(RecordType());
    return position;
}

template<typename TData>
typename Node<TData>::PositionType Node<TData>::Position() const
{
    if (!m_point) return -1;
    return m_point->Position();
}

template<typename TData>
bool Node<TData>::HasParent() const
{
    return m_parent != nullptr;
}

template<typename TData>
typename Node<TData>::EdgeInterfaceType & 
Node<TData>::Parent()
{
    return *m_parent;
}

template<typename TData>
const typename Node<TData>::EdgeInterfaceType & 
Node<TData>::Parent() const
{
    return *m_parent;
}

template<typename TData>
typename Node<TData>::PositionType 
Node<TData>::ParentPosition() const
{
    return m_point->Record().Parent();
}

template<typename TData>
void Node<TData>::ParentPosition(const PositionType & new_position)
{
    if (!m_point->Position() == -1) return;
    m_point->Record().Parent(new_position);
}

template<typename TData>
bool Node<TData>::HasLeft() const
{
    return m_left != nullptr;
}

template<typename TData>
typename Node<TData>::EdgeInterfaceType & 
Node<TData>::Left()
{
    return *m_left;
}

template<typename TData>
const typename Node<TData>::EdgeInterfaceType & 
Node<TData>::Left() const
{
    return *m_left;
}

template<typename TData>
typename Node<TData>::PositionType 
Node<TData>::LeftPosition() const
{
    return m_point->Record().Left();
}

template<typename TData>
void Node<TData>::LeftPosition(const PositionType & new_position)
{
    if (!m_point->Position() == -1) return;
    m_point->Record().Left(new_position);
}

template<typename TData>
bool Node<TData>::HasRight() const
{
    return m_right != nullptr;
}

template<typename TData>
typename Node<TData>::EdgeInterfaceType & 
Node<TData>::Right()
{
    return *m_right;
}

template<typename TData>
const typename Node<TData>::EdgeInterfaceType & 
Node<TData>::Right() const
{
    return *m_right;
}

template<typename TData>
typename Node<TData>::PositionType 
Node<TData>::RightPosition() const
{
    return m_point->Record().Right();
}

template<typename TData>
void Node<TData>::RightPosition(const PositionType & new_position)
{
    if (m_point->Position() == -1) return;
    m_point->Record().Right(new_position);
}

template<typename TData>
typename Node<TData>::HightValueType Node<TData>::Hight() const
{
    return m_point->Record().Hight();
}

template<typename TData>
void Node<TData>::Hight(const HightValueType & set)
{
    if (m_point->Position() == -1) return;
    m_point->Record().Hight(set);
}

template<typename TData>
bool Node<TData>::IsDeleted() const
{
    return m_point->Record().Flags() & RecordType::delete_flag;
}

template<typename TData>
void Node<TData>::Delete()
{
    if (m_point && m_point->Position() == -1) return;
    m_point->Record().Flags(m_point->Record().Flags() | 
        RecordType::delete_flag);
}

template<typename TData>
TData Node<TData>::Data() const
{
    return m_point->Record().Data();
}

template<typename TData>
void Node<TData>::Data(const TData & data)
{
    if (m_point && m_point->Position() == -1) return;
    m_point->Record().Data(data);
}

template<typename TData>
int Node<TData>::Balance() const
{
    if (!m_point || m_point->Position() == -1) return 0;
    HightValueType left = 0, right = 0;
    if (HasRight()) right = (*m_right)->Hight();
    if (HasLeft()) left = (*m_left)->Hight();
    return right - left;
}

template<typename TData>
void Node<TData>::Synchronize(const bool & force)
{
    if (m_point->Position() == -1) return;
    auto & rec = m_point->Record();
    m_fileFormatLinear->SeekPosition(m_point->Position());
    if (force || rec.IsInitial())
    {
        m_fileFormatLinear->CurrentGet(rec);
    }
    else if (rec.IsOutOfSynchronization())
    {
        m_fileFormatLinear->CurrentPut(rec);
    }
}

template<typename TData>
void Node<TData>::Synchronize(const bool & force) const
{
    const_cast<Node<TData>*>(this)->Synchronize(force);
}

template<typename TData>
bool Node<TData>::operator==(const NodeInterfaceType & other) const
{
    auto cast_node = dynamic_cast<Node<TData> const*>(&other);
    if (!cast_node || m_file != cast_node->m_file ||
        m_fileFormatLinear != cast_node->m_fileFormatLinear) return false;
    return m_point == cast_node->m_point;
}

template<typename TData>
bool Node<TData>::operator!=(const NodeInterfaceType & other) const
{
    return !(*this == other);
}

template<typename TData>
Node<TData>::operator bool() const
{
    return IsValid() && m_point &&
        m_point->Position() != -1 && !IsDeleted();
}

} //file

} //!imp

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_IMP_FILE_NODE_H_