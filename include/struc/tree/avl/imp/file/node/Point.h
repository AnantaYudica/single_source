#ifndef STRUC_TREE_AVL_IMP_FILE_NODE_POINT_H_
#define STRUC_TREE_AVL_IMP_FILE_NODE_POINT_H_

#include "Point.defn.h"

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
namespace node
{

template<typename TData>
Point<TData>::Point(const PositionType & pos) :
    m_position(pos),
    m_record()
{}

template<typename TData>
Point<TData>::~Point()
{
    m_position = -1;
}

template<typename TData>
Point<TData>::Point(const Point<TData> & cpy) :
    m_position(cpy.m_position),
    m_record(cpy.m_record)
{}

template<typename TData>
Point<TData>::Point(Point<TData> && mov) :
    m_position(mov.m_position),
    m_record(std::move(mov.m_record))
{
    mov.m_position = -1;
}

template<typename TData>
Point<TData> & Point<TData>::operator=(const Point<TData> & cpy)
{
    m_position = cpy.m_position;
    m_record = cpy.m_record;
    return *this;
}

template<typename TData>
Point<TData> & Point<TData>::operator=(Point<TData> && mov)
{
    m_position = mov.m_position;
    m_record = std::move(mov.m_record);
    mov.m_position = -1;
    return *this;
}

template<typename TData>
typename Point<TData>::PositionType 
Point<TData>::Position() const
{
    return m_position;
}

template<typename TData>
typename Point<TData>::RecordType & 
Point<TData>::Record()
{
    return m_record;
}

} //!node

} //!file

} //!imp

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_IMP_FILE_NODE_POINT_H_
