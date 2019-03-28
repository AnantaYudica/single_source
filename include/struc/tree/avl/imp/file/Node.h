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
    m_file(nullptr),
    m_fileFormatLinear(),
    m_parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{}

template<typename TData>
Node<TData>::Node(FileInterfacePointerType file) :
    m_record(),
    m_file(file),
    m_fileFormatLinear(file, defn::rec::Size<RecordType>::Value),
    m_parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{}

template<typename TData>
Node<TData>::Node(FileInterfacePointerType file, PositionType position) :
    m_record(),
    m_file(file),
    m_fileFormatLinear(file, defn::rec::Size<RecordType>::Value),
    m_parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{
    if (position != -1) 
    {
        m_fileFormatLinear.SeekPosition(position);
        m_fileFormatLinear.CurrentGet(m_record);
        m_parent = new Pointer<TData>(this, Pointer<TData>::Way::parent);
        m_right = new Pointer<TData>(this, Pointer<TData>::Way::right);
        m_left = new Pointer<TData>(this, Pointer<TData>::Way::left);
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
    m_file(cpy.m_file),
    m_fileFormatLinear(cpy.m_fileFormatLinear),
    m_parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{
    m_parent = new Pointer<TData>(this, Pointer<TData>::Way::parent);
    m_right = new Pointer<TData>(this, Pointer<TData>::Way::right);
    m_left = new Pointer<TData>(this, Pointer<TData>::Way::left);
}

template<typename TData>
Node<TData>::Node(Node<TData> && mov) :
    m_record(std::move(mov.m_record)),
    m_file(std::move(mov.m_file)),
    m_fileFormatLinear(std::move(cpy.m_fileFormatLinear)),
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
    m_file = cpy.m_file;
    m_fileFormatLinear = cpy.m_fileFormatLinear;
    m_parent = new Pointer<TData>(this, Pointer<TData>::Way::parent);
    m_right = new Pointer<TData>(this, Pointer<TData>::Way::right);
    m_left = new Pointer<TData>(this, Pointer<TData>::Way::left);
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::operator=(Node<TData> && mov)
{
    m_record = std::move(mov.m_record);
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
    return *this;
}

template<typename TData>
void Node<TData>::Default()
{
    m_record = RecordType();
    m_file = nullptr;
    m_fileFormatLinear = FileFormatLinearType();
    m_parent = nullptr;
    m_right = nullptr;
    m_left = nullptr;
}

template<typename TData>
Node<TData> Node<TData>::Instance(PositionType position) const
{
    return Node<TData>{m_file, position};
}

template<typename TData>
typename Node<TData>::InterfacePointerType 
Node<TData>::MakeCopy() const
{
    return InterfacePointerType(new Node<TData>(*this));
}

template<typename TData>
typename Node<TData>::InterfacePointerType Node<TData>::MakeMove()
{
    return InterfacePointerType(new Node<TData>(std::move(*this)));
}

template<typename TData>
Node<TData> & Node<TData>::Emplace(const TData & data)
{
    if (!m_file || !m_file->IsOpen()) return *this;
    if (!(m_record.Status() & defn::rec::Status::initial))
        RecordType::Default(m_record);
    m_fileFormatLinear.SeekOffset(0, WayType::end);
    m_record.Data(data);
    m_fileFormatLinear.CurentPut(m_record);
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::Displace()
{
    if (!m_file || !m_file->IsOpen() || m_record.Current() == -1) return *this;
    m_record.Flags(m_record.Flags() | RecordType::DeleteFlag);
    m_fileFormatLinear.SeekPosition(m_record.Current());
    m_fileFormatLinear.CurentPut(m_record);
    return *this;
}

template<typename TData>
typename Node<TData>::PositionType Node<TData>::Position() const
{
    return m_record.Current();
}

template<typename TData>
typename Node<TData>::PointerInterfaceType & 
Node<TData>::Parent()
{
    if (!m_parent) 
        m_parent = new Pointer<TData>(this, Pointer<TData>::Way::parent);
    return *m_parent;
}

template<typename TData>
const typename Node<TData>::PointerInterfaceType & 
Node<TData>::Parent() const
{
    if (!m_parent) 
        const_cast<Pointer<TData> *>(m_parent) = new Pointer<TData>(this, 
            Pointer<TData>::Way::parent);
    return *m_parent;
}

template<typename TData>
void Node<TData>::Parent(const PositionType & new_position)
{
    m_record.Parent(new_position);
}

template<typename TData>
typename Node<TData>::PointerInterfaceType & 
Node<TData>::Right()
{
    if (!m_right) 
        m_right = new Pointer<TData>(this, Pointer<TData>::Way::right);
    return *m_right;
}

template<typename TData>
const typename Node<TData>::PointerInterfaceType & 
Node<TData>::Right() const
{
    if (!m_right) 
        const_cast<Pointer<TData> *>(m_right) = new Pointer<TData>(this, 
            Pointer<TData>::Way::right);
    return *m_right;
}

template<typename TData>
void Node<TData>::Right(const PositionType & new_position)
{
    m_record.Right(new_position);
}

template<typename TData>
typename Node<TData>::PointerInterfaceType & 
Node<TData>::Left()
{
    if (!m_left) 
        m_left = new Pointer<TData>(this, Pointer<TData>::Way::left);
    return *m_left;
}

template<typename TData>
const typename Node<TData>::PointerInterfaceType & 
Node<TData>::Left() const
{
    if (!m_left) 
        const_cast<Pointer<TData> *>(m_left) = new Pointer<TData>(this, 
            Pointer<TData>::Way::left);
    return *m_left;
}

template<typename TData>
void Node<TData>::Left(const PositionType & new_position)
{
    m_record.Left(new_position);
}

template<typename TData>
int Node<TData>::Hight() const
{
    return m_hight.Hight();
}

template<typename TData>
int Node<TData>::Hight(HightValueType & set)
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
    m_fileFormatLinear.CurrentPut(m_record);
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
        m_record == node->m_record;
}

template<typename TData>
bool Node<TData>::operator!=(const NodeInterfaceType & other) const
{
    auto node = dynamic_cast<const Node<TData> *>(&other);
    return !node || m_file != node->m_file ||
        m_record != node->m_record;
}

template<typename TData>
Node<TData>::operator bool() const
{
    return m_file && m_record.Current() != -1 &&
        m_file->IsOpen();
}

} //file

} //!imp

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_IMP_FILE_NODE_H_