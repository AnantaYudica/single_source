#ifndef STRUC_TREE_AVL_IMP_FILE_NODE_H_
#define STRUC_TREE_AVL_IMP_FILE_NODE_H_

#include "Node.defn.h"
#include "Pointer.defn.h"

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
Node<TData>::Node() :
    m_record(),
    m_position(-1),
    m_file(nullptr),
    m_fileFormatLinear(nullptr),
    m_parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{}

template<typename TData>
Node<TData>::Node(FileInterfacePointerType file, 
    FileFormatLinearPointerType format) :
        m_record(),
        m_position(-1),
        m_file(file),
        m_fileFormatLinear(format),
        m_parent(nullptr),
        m_right(nullptr),
        m_left(nullptr)
{}

template<typename TData>
Node<TData>::Node(FileInterfacePointerType file, 
    FileFormatLinearPointerType format, const PositionType & position) :
        m_record(),
        m_position(position),
        m_file(file),
        m_fileFormatLinear(format),
        m_parent(nullptr),
        m_right(nullptr),
        m_left(nullptr)
{
    if (position != -1 && IsValid()) 
    {
        m_fileFormatLinear->SeekPosition(position);
        m_fileFormatLinear->CurrentGet(m_record);
        m_parent = new Pointer<TData>(this, Pointer<TData>::WayType::parent);
        m_right = new Pointer<TData>(this, Pointer<TData>::WayType::right);
        m_left = new Pointer<TData>(this, Pointer<TData>::WayType::left);
    }
}

template<typename TData>
Node<TData>::~Node()
{
    if (m_parent) delete m_parent;
    m_parent = nullptr;
    if (m_right) delete m_right;
    m_right = nullptr;
    if (m_left) delete m_left;
    m_left = nullptr;
}

template<typename TData>
Node<TData>::Node(const Node<TData> & cpy) :
    m_record(cpy.m_record),
    m_position(cpy.m_position),
    m_file(cpy.m_file),
    m_fileFormatLinear(cpy.m_fileFormatLinear),
    m_parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{
    m_parent = new Pointer<TData>(this, Pointer<TData>::WayType::parent);
    m_right = new Pointer<TData>(this, Pointer<TData>::WayType::right);
    m_left = new Pointer<TData>(this, Pointer<TData>::WayType::left);
}

template<typename TData>
Node<TData>::Node(Node<TData> && mov) :
    m_record(std::move(mov.m_record)),
    m_position(std::move(mov.m_position)),
    m_file(std::move(mov.m_file)),
    m_fileFormatLinear(std::move(mov.m_fileFormatLinear)),
    m_parent(std::move(mov.m_parent)),
    m_right(std::move(mov.m_right)),
    m_left(std::move(mov.m_left))
{
    mov.Default();
}

template<typename TData>
Node<TData> & Node<TData>::operator=(const Node<TData> & cpy)
{
    m_record = cpy.m_record;
    m_position = cpy.m_position;
    m_file = cpy.m_file;
    m_fileFormatLinear = cpy.m_fileFormatLinear;
    m_parent = new Pointer<TData>(this, Pointer<TData>::WayType::parent);
    m_right = new Pointer<TData>(this, Pointer<TData>::WayType::right);
    m_left = new Pointer<TData>(this, Pointer<TData>::WayType::left);
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::operator=(Node<TData> && mov)
{
    m_record = std::move(mov.m_record);
    m_position = std::move(mov.m_position);
    m_file = std::move(mov.m_file);
    m_fileFormatLinear = std::move(mov.m_fileFormatLinear);
    m_parent = std::move(mov.m_parent);
    m_right = std::move(mov.m_right);
    m_left = std::move(mov.m_left);
    mov.Default();
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::operator=(const NodeInterfaceType & cpy)
{
    auto ptr = dynamic_cast<const Node<TData> *>(&cpy);
    if (ptr) return *this = *ptr;
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::operator=(NodeInterfaceType && mov)
{
    auto ptr = dynamic_cast<Node<TData> *>(&mov);
    if (ptr) return *this = std::move(*ptr);
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::operator=(const TData & data)
{
    m_record.Data(data);
    m_fileFormatLinear->CurrentPut(m_record);
    return *this;
}

template<typename TData>
void Node<TData>::Default()
{
    m_record = RecordType();
    m_position = -1;
    m_file = nullptr;
    m_fileFormatLinear = nullptr;
    m_parent = nullptr;
    m_right = nullptr;
    m_left = nullptr;
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
typename Node<TData>::NodeInterfacePointerType 
Node<TData>::MakeCopy() const
{
    return NodeInterfacePointerType(new Node<TData>(*this));
}

template<typename TData>
typename Node<TData>::NodeInterfacePointerType Node<TData>::MakeMove()
{
    return NodeInterfacePointerType(new Node<TData>(std::move(*this)));
}

template<typename TData>
Node<TData> & Node<TData>::Emplace(const TData & data)
{
    if (!IsValid()) return *this;
    if (!m_record.IsInitial())
        m_record = RecordType();
    m_position = m_fileFormatLinear->Size();
    m_fileFormatLinear->SeekOffset(0, WayType::end);
    m_record.Data(data);
    m_fileFormatLinear->CurrentPut(m_record);
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::Displace()
{
    if (!IsValid() && m_position == -1) return *this;
    m_record.Flags(m_record.Flags() | RecordType::delete_flag);
    Synchronize();
    m_position = -1;
    return *this;
}

template<typename TData>
typename Node<TData>::PositionType Node<TData>::Position() const
{
    return m_position;
}

template<typename TData>
typename Node<TData>::PointerInterfaceType & 
Node<TData>::Parent()
{
    if (!m_parent) 
        m_parent = new Pointer<TData>(this, Pointer<TData>::WayType::parent);
    return *m_parent;
}

template<typename TData>
const typename Node<TData>::PointerInterfaceType & 
Node<TData>::Parent() const
{
    if (!m_parent) *const_cast<Pointer<TData> **>(&m_parent) = 
        new Pointer<TData>(const_cast<Node<TData> *>(this), 
        Pointer<TData>::WayType::parent);
    return *m_parent;
}

template<typename TData>
typename Node<TData>::PositionType Node<TData>::ParentPosition() const
{
    return m_record.Parent();
}

template<typename TData>
void Node<TData>::ParentPosition(const PositionType & new_position)
{
    m_record.Parent(new_position);
    Synchronize();
}

template<typename TData>
typename Node<TData>::PointerInterfaceType & 
Node<TData>::Right()
{
    if (!m_right) 
        m_right = new Pointer<TData>(this, Pointer<TData>::WayType::right);
    return *m_right;
}

template<typename TData>
const typename Node<TData>::PointerInterfaceType & 
Node<TData>::Right() const
{
    if (!m_right) *const_cast<Pointer<TData> **>(&m_right) = 
        new Pointer<TData>(const_cast<Node<TData> *>(this), 
        Pointer<TData>::WayType::right);
    return *m_right;
}

template<typename TData>
typename Node<TData>::PositionType Node<TData>::RightPosition() const
{
    return m_record.Right();
}

template<typename TData>
void Node<TData>::RightPosition(const PositionType & new_position)
{
    m_record.Right(new_position);
    Synchronize();
}

template<typename TData>
typename Node<TData>::PointerInterfaceType & 
Node<TData>::Left()
{
    if (!m_left) 
        m_left = new Pointer<TData>(this, Pointer<TData>::WayType::left);
    return *m_left;
}

template<typename TData>
const typename Node<TData>::PointerInterfaceType & 
Node<TData>::Left() const
{
    if (!m_left) *const_cast<Pointer<TData> **>(&m_left) = 
        new Pointer<TData>(const_cast<Node<TData> *>(this), 
        Pointer<TData>::WayType::left);
    return *m_left;
}

template<typename TData>
typename Node<TData>::PositionType Node<TData>::LeftPosition() const
{
    return m_record.Left();
}

template<typename TData>
void Node<TData>::LeftPosition(const PositionType & new_position)
{
    m_record.Left(new_position);
    Synchronize();
}

template<typename TData>
int Node<TData>::Hight() const
{
    return m_record.Hight();
}

template<typename TData>
int Node<TData>::Hight(const HightValueType & set)
{
    m_record.Hight(set);
}

template<typename TData>
int Node<TData>::Balance()
{
    return Right()->Hight() - Left()->Hight();
}

template<typename TData>
int Node<TData>::Balance() const
{
    return Right()->Hight() - Left()->Hight();
}

template<typename TData>
void Node<TData>::Swap(NodeInterfaceType & other)
{
    auto node = dynamic_cast<Node<TData> *>(&other);
    if (!node) return;
    Node<TData> cpy{*node};
    *this = std::move(*node);
    *node = std::move(cpy);
}

template<typename TData>
bool Node<TData>::IsDeleted()
{
    return m_record.Flags() & RecordType::delete_flag;
}

template<typename TData>
void Node<TData>::Synchronize()
{
    m_fileFormatLinear->SeekPosition(m_position);
    m_fileFormatLinear->CurrentPut(m_record);
}

template<typename TData>
TData Node<TData>::operator*() const
{
    return m_record.Data();
}

template<typename TData>
bool Node<TData>::operator==(const NodeInterfaceType & other) const
{
    auto node = dynamic_cast<const Node<TData> *>(&other);
    return node && m_file == node->m_file &&
        m_fileFormatLinear == node->m_fileFormatLinear &&
        m_record == node->m_record;
}

template<typename TData>
bool Node<TData>::operator!=(const NodeInterfaceType & other) const
{
    auto node = dynamic_cast<const Node<TData> *>(&other);
    return !node || m_file != node->m_file ||
        m_fileFormatLinear != node->m_fileFormatLinear ||
        m_record != node->m_record;
}

template<typename TData>
Node<TData>::operator bool() const
{
    return m_file && m_position != -1 &&
        m_file->IsOpen() && m_fileFormatLinear &&
        m_file == m_fileFormatLinear->File();
}

} //file

} //!imp

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_IMP_FILE_NODE_H_