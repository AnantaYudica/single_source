#ifndef FILE_FORMAT_TREE_AVL_POINTER_H_
#define FILE_FORMAT_TREE_AVL_POINTER_H_

#include "Pointer.defn.h"
#include "Node.defn.h"

#include <fstream>

namespace file
{
namespace format
{
namespace tree
{
namespace avl
{

template<typename TData>
Pointer<TData>::Pointer() :
    m_point(nullptr),
    m_next(nullptr),
    m_way(Way::undefined)
{}

template<typename TData>
Pointer<TData>::Pointer(NodeType * base, Way way) :
    m_point(base),
    m_next(nullptr),
    m_way(way)
{}

template<typename TData>
Pointer<TData>::~Pointer()
{
    if(m_next) delete m_next;
}

template<typename TData>
Pointer<TData>::Pointer(const Pointer & cpy) :
    m_point(cpy.m_point),
    m_next(nullptr),
    m_way(cpy.m_way)
{}

template<typename TData>
Pointer<TData>::Pointer(Pointer && mov) :
    m_point(mov.m_point),
    m_next(mov.m_next),
    m_way(mov.m_way)
{
    mov.Default();
}

template<typename TData>
Pointer<TData> & Pointer<TData>::operator=(const Pointer<TData> & cpy)
{
    m_point = cpy.m_point;
    m_way = cpy.m_way;
    Reset();
    return *this;
}

template<typename TData>
Pointer<TData> & Pointer<TData>::operator=(Pointer<TData> && mov)
{
    m_point = mov.m_point;
    m_next = mov.m_next;
    m_way = mov.m_way;
    mov.Default();
    return *this;
}

template<typename TData>
Pointer<TData> & Pointer<TData>::operator=(const PointerInterfaceType & cpy)
{
    auto ptr = dynamic_cast<const Pointer<TData> *>(&cpy);
    if (ptr) *this = *ptr;
    return *this;
}

template<typename TData>
Pointer<TData> & Pointer<TData>::operator=(PointerInterfaceType && mov)
{
    auto ptr = dynamic_cast<Pointer<TData> *>(&mov);
    if (ptr) *this = std::move(*ptr);
    return *this;
}

template<typename TData>
void Pointer<TData>::Default()
{
    m_point = nullptr;
    m_next = nullptr;
    m_way = Way::undefined;
}

template<typename TData>
void Pointer<TData>::Set(const PositionType & pos)
{
    switch(m_way)
    {
        case Way::parent:
            m_point->Parent(pos);
            break;
        case Way::left:
            m_point->Left(pos);
            break;
        case Way::right:
            m_point->Right(pos);
            break;
    };
}

template<typename TData>
typename Pointer<TData>::PositionType Pointer<TData>::Get()
{
    switch(m_way)
    {
        case Way::parent:
            return m_point->Parent();
        case Way::left:
            return m_point->Left();
        case Way::right:
            return m_point->Right();
    }
    return -1;
}

template<typename TData>
typename Pointer<TData>::NodeType * Pointer<TData>::Node()
{
    if (m_next != nullptr) return m_next;
    const auto pos = Get();
    if (pos != -1) m_next = new NodeType(std::move(m_point->Instance(pos)));
    return m_next;
}

template<typename TData>
void Pointer<TData>::Reset()
{
    if (!m_next) return;
    delete m_next;
    m_next = nullptr;
}

template<typename TData>
bool Pointer<TData>::IsNull() const
{
    return m_point == nullptr || m_way == Way::undefined;
}

template<typename TData>
typename Pointer<TData>::PointerInterfaceType & 
Pointer<TData>::Emplace(const DataType & data)
{
    if (IsNull()) return *this;
    Reset();
    m_next = new NodeType(std::move(m_point->Instance()));
    m_next->Emplace(data);
    m_next->Parent(m_point->Position());
    Set(m_next->Position());
    return *this;
}

template<typename TData>
typename Pointer<TData>::PointerInterfaceType &  Pointer<TData>::Displace()
{
    if (IsNull() || !m_next) return *this;
    Reset();
    Set(-1);
    return *this;
}

template<typename TData>
Pointer<TData> & Pointer<TData>::operator=(const NodeInterfaceType & other)
{
    if (IsNull()) return *this;
    auto ptr = dynamic_cast<const NodeType *>(&other);
    if (!ptr) return *this;
    Reset();
    Set(ptr->Position());
    return *this;
}

template<typename TData>
Pointer<TData>::operator bool() const
{
    return  m_point != nullptr && m_way != Way::undefined;
}

template<typename TData>
typename Pointer<TData>::NodeInterfaceType & Pointer<TData>::operator*()
{
    static NodeType def{m_point->Instance()};
    auto node = Node();
    if (node) return *node;
    return def;
}

template<typename TData>
const typename Pointer<TData>::NodeInterfaceType & 
Pointer<TData>::operator*() const
{
    static NodeType def{m_point->Instance()};
    if (m_next) return *m_next;
    return def;
}

template<typename TData>
typename Pointer<TData>::NodeInterfaceType * Pointer<TData>::operator->()
{
    static NodeType def{m_point->Instance()};
    auto node = Node();
    if (node) return node;
    return &def;
}

template<typename TData>
const typename Pointer<TData>::NodeInterfaceType *
Pointer<TData>::operator->() const
{
    static NodeType def{m_point->Instance()};
    if (m_next) return m_next;
    return &def;
}

template<typename TData>
bool Pointer<TData>::operator==(const PointerInterfaceType &  other) const
{
    auto ptr = dynamic_cast<const Pointer<TData> *>(&other);
    return ptr && m_way == ptr->m_way && 
        m_point == ptr->m_point;
}

template<typename TData>
bool Pointer<TData>::operator==(const NodeInterfaceType & other) const
{
    return !IsNull() && m_next && *m_next == other;
}

template<typename TData>
bool Pointer<TData>::operator!=(const PointerInterfaceType & other) const
{
    auto ptr = dynamic_cast<const Pointer<TData> *>(&other);
    return !ptr || m_way != ptr->m_way || 
        m_point != ptr->m_point;
}

template<typename TData>
bool Pointer<TData>::operator!=(const NodeInterfaceType & other) const
{
    return IsNull() || !m_next || *m_next != other;
}

} //!avl

} //!tree

} //!format

} //!file

#endif //!FILE_FORMAT_TREE_AVL_POINTER_H_
