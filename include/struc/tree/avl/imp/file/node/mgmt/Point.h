#ifndef STRUC_TREE_AVL_IMP_FILE_NODE_MGMT_POINT_H_
#define STRUC_TREE_AVL_IMP_FILE_NODE_MGMT_POINT_H_

#include "../Point.defn.h"

#include <vector>

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
namespace mgmt
{

template<typename TData>
class Point
{
public:
    typedef node::Point<TData> ElementType;
    typedef typename ElementType::PositionType PositionType;
public:
   static Point<TData> & GetInstance();
private:
    std::vector<ElementType *> m_elemets;
    std::vector<std::size_t> m_counts;
private:
    Point();
public:
    ~Point();
public:
    Point(const Point<TData> &) = delete;
    Point(Point<TData> &&) = delete;
public:
    Point<TData> & operator=(const Point<TData> &) = delete;
    Point<TData> & operator=(Point<TData> &&) = delete;
public:
    ElementType * Allocate(const PositionType & pos);
    ElementType * Allocate(ElementType * pointer);
    void Deallocate(ElementType * pointer);

};

template<typename TData>
Point<TData> & Point<TData>::GetInstance()
{
    static Point<TData> instance;
    return instance;
}

template<typename TData>
Point<TData>::Point() :
    m_elemets(),
    m_counts()
{}

template<typename TData>
Point<TData>::~Point()
{
    for (std::size_t i = 0; i < m_elemets.size(); ++i)
    {
        delete m_elemets[i];
    }
}

template<typename TData>
typename Point<TData>::ElementType * 
Point<TData>::Allocate(const PositionType & pos)
{
    for (std::size_t i = 0; i < m_elemets.size(); ++i)
    {
        if (m_elemets[i]->Position() == pos)
        {
            ++m_counts[i];
            return m_elemets[i];
        }
    }
    m_elemets.push_back(new ElementType(pos));
    m_counts.push_back(1);
    return m_elemets.back();
}

template<typename TData>
typename Point<TData>::ElementType * 
Point<TData>::Allocate(ElementType * pointer)
{
    if (!pointer) return nullptr;
    for (std::size_t i = 0; i < m_elemets.size(); ++i)
    {
        if (m_elemets[i] == pointer)
        {
            ++m_counts[i];
            return pointer;
        }
    }
    m_elemets.push_back(new ElementType(pointer->Position()));
    m_counts.push_back(1);
    return m_elemets.back();
}

template<typename TData>
void Point<TData>::Deallocate(ElementType * pointer)
{
    if (!pointer) return;
    for (std::size_t i = 0; i < m_elemets.size(); ++i)
    {
        if (m_elemets[i] == pointer)
        {
            --m_counts[i];
            if (m_counts[i] == 0)
            {
                delete m_elemets[i];
                m_elemets.erase(m_elemets.begin() + i);
                m_counts.erase(m_counts.begin() + i);
            }
        }
    }
}

} //!mgmt

} //!node

} //!file

} //!imp

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_IMP_FILE_NODE_MGMT_POINT_H_
