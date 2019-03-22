#ifndef STRUC_TREE_AVL_IMP_FILE_NODE_H_
#define STRUC_TREE_AVL_IMP_FILE_NODE_H_

#include "Node.defn.h"
#include "Pointer.defn.h"

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
    m_file(nullptr),
    m_position(-1),
    m_flag(0),
    m_hight(0),
    m_parentPosition(-1),
    m_rightPosition(-1),
    m_leftPosition(-1),
    m_parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{}

template<typename TData>
Node<TData>::Node(FileInterfacePointerType file) :
    m_file(file),
    m_position(-1),
    m_flag(0),
    m_hight(0),
    m_parentPosition(-1),
    m_rightPosition(-1),
    m_leftPosition(-1),
    m_parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{}

template<typename TData>
Node<TData>::Node(FileInterfacePointerType file, FilePositionType position) :
        m_file(file),
        m_position(position),
        m_flag(0),
        m_hight(0),
        m_parentPosition(-1),
        m_rightPosition(-1),
        m_leftPosition(-1),
        m_parent(nullptr),
        m_right(nullptr),
        m_left(nullptr)
{
    if (position != -1) Read();
    m_parent = new PointerType(this, PointerType::Way::parent);
    m_right = new PointerType(this, PointerType::Way::right);
    m_left = new PointerType(this, PointerType::Way::left);
}

template<typename TData>
Node<TData>::~Node()
{
    if (m_parent) delete m_parent;
    if (m_right) delete m_right;
    if (m_left) delete m_left;
}

template<typename TData>
Node<TData>::Node(const Node<TData> & cpy) :
    m_file(cpy.m_file),
    m_position(cpy.m_position),
    m_flag(cpy.m_flag),
    m_hight(cpy.m_hight),
    m_parentPosition(cpy.m_parentPosition),
    m_rightPosition(cpy.m_rightPosition),
    m_leftPosition(cpy.m_leftPosition),
    m_parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{
    m_parent = new PointerType(this, PointerType::Way::parent);
    m_right = new PointerType(this, PointerType::Way::right);
    m_left = new PointerType(this, PointerType::Way::left);
}

template<typename TData>
Node<TData>::Node(Node<TData> && mov) :
    m_file(move(mov.m_file)),
    m_position(mov.m_position),
    m_flag(mov.m_flag),
    m_hight(mov.m_hight),
    m_parentPosition(mov.m_parentPosition),
    m_rightPosition(mov.m_rightPosition),
    m_leftPosition(mov.m_leftPosition),
    m_parent(mov.m_parent),
    m_right(mov.m_right),
    m_left(mov.m_left)
{
    mov.Default();
}

template<typename TData>
Node<TData> & Node<TData>::operator=(const Node<TData> & cpy)
{
    m_file = cpy.m_file;
    m_position = cpy.m_position;
    m_flag = cpy.m_flag;
    m_hight = cpy.m_hight;
    m_parentPosition = cpy.m_parentPosition;
    m_rightPosition = cpy.m_rightPosition;
    m_leftPosition = cpy.m_leftPosition;
    m_parent = new PointerType(this, PointerType::Way::parent);
    m_right = new PointerType(this, PointerType::Way::right);
    m_left = new PointerType(this, PointerType::Way::left);
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::operator=(Node<TData> && mov)
{
    m_file = move(mov.m_file);
    m_position = mov.m_position;
    m_flag = mov.m_flag;
    m_hight = mov.m_hight;
    m_parentPosition = mov.m_parentPosition;
    m_rightPosition = mov.m_rightPosition;
    m_leftPosition = mov.m_leftPosition;
    m_parent = mov.m_parent;
    m_right = mov.m_right;
    m_left = mov.m_left;

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
void Node<TData>::Default()
{
    m_file = nullptr;
    m_position = -1;
    m_flag = 0;
    m_hight = 0;
    m_parentPosition = -1;
    m_rightPosition = -1;
    m_leftPosition = -1;
    m_parent = nullptr;
    m_right = nullptr;
    m_left = nullptr;
}

template<typename TData>
template<typename TValue>
TValue Node<TData>::ReadValue(FilePositionType position)
{
    char buff[sizeof(TValue)];
    TValue value;
    m_file->SeekPosition(position);
    m_file->Get(buff, sizeof(buff));
    value = *reinterpret_cast<TValue *>(buff);
    return value;
}

template<typename TData>
template<typename TValue>
void Node<TData>::WriteValue(const TValue & val, 
    FilePositionType position)
{
    char buff[sizeof(TValue)];
    memcpy(buff, (const char *)&val, sizeof(TValue));
    m_file->SeekPosition(position);
    m_file->Put(buff, sizeof(buff));
}

template<typename TData>
void Node<TData>::Read(std::uint8_t flags)
{
    if (flags & ms_rwFlag)
        m_flag = ReadValue<FlagValueType>(m_position + ms_flagOffset);
    if (flags & ms_rwHight)
        m_hight = ReadValue<HightValueType>(m_position + ms_hightOffset);
    if (flags & ms_rwParentPosition)
        m_parentPosition = ReadValue<PositionType>(m_position + 
            ms_parentPositionOffset);
    if (flags & ms_rwRightPosition)
        m_rightPosition = ReadValue<PositionType>(m_position + 
            ms_rightPositionOffset);
    if (flags & ms_rwLeftPosition)
        m_leftPosition = ReadValue<PositionType>(m_position + 
            ms_leftPositionOffset);
    if (flags & ms_rwData)
        m_data = ReadValue<DataType>(m_position + ms_dataOffset);
}

template<typename TData>
void Node<TData>::Write(std::uint8_t flags)
{
    if (flags & ms_rwFlag)
        WriteValue<FlagValueType>(m_flag, m_position + ms_flagOffset);
    if (flags & ms_rwHight)
        WriteValue<HightValueType>(m_hight, m_position + ms_hightOffset);
    if (flags & ms_rwParentPosition)
        WriteValue<PositionType>(m_parentPosition, m_position + 
            ms_parentPositionOffset);
    if (flags & ms_rwRightPosition)
        WriteValue<PositionType>(m_rightPosition, m_position + 
            ms_rightPositionOffset);
    if (flags & ms_rwLeftPosition)
        WriteValue<PositionType>(m_leftPosition, m_position + 
            ms_leftPositionOffset);
    if (flags & ms_rwData)
        WriteValue<DataType>(m_data, m_position + ms_dataOffset);
}

template<typename TData>
Node<TData> Node<TData>::Instance(FilePositionType position) const
{
    return Node<TData>{m_file, position};
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
    if (!m_file || !m_file->IsOpen()) return *this;
    m_flag = 0;
    m_hight = 0;
    m_parentPosition = -1;
    m_rightPosition = -1;
    m_leftPosition = -1;
    m_data = data;
    if (m_position == -1)
        m_position = m_file->SeekOffset(0, ::intf::file::Way::end);
    Write();
    return *this;
}

template<typename TData>
Node<TData> & Node<TData>::Displace()
{
    if (!m_file || !m_file->IsOpen()) return *this;
    m_flag |= 0x01;
    Write(0x01);
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
        m_parent = new PointerType(this, PointerType::Way::parent);
    return *m_parent;
}

template<typename TData>
const typename Node<TData>::PointerInterfaceType & 
Node<TData>::Parent() const
{
    static PointerType def{};
    return m_parent ? *m_parent : def;
}

template<typename TData>
void Node<TData>::Parent(const PositionType & new_position)
{
    if (m_parentPosition == new_position) return;
    m_parentPosition = new_position;
    Write(ms_rwParentPosition);
}

template<typename TData>
typename Node<TData>::PointerInterfaceType & 
Node<TData>::Right()
{
    if (!m_right) 
        m_right = new PointerType(this, PointerType::Way::right);
    return *m_right;
}

template<typename TData>
const typename Node<TData>::PointerInterfaceType & 
Node<TData>::Right() const
{
    static PointerType def{};
    return m_right ? *m_right : def;
}

template<typename TData>
void Node<TData>::Right(const PositionType & new_position)
{
    if (m_rightPosition == new_position) return;
    m_rightPosition = new_position;
    Write(ms_rwRightPosition);
}

template<typename TData>
typename Node<TData>::PointerInterfaceType & 
Node<TData>::Left()
{
    if (!m_left) 
        m_left = new PointerType(this, PointerType::Way::left);
    return *m_left;
}

template<typename TData>
const typename Node<TData>::PointerInterfaceType & 
Node<TData>::Left() const
{
    static PointerType def{};
    return m_left ? *m_left : def;
}

template<typename TData>
void Node<TData>::Left(const PositionType & new_position)
{
    if (m_leftPosition == new_position) return;
    m_leftPosition = new_position;
    Write(ms_rwLeftPosition);
}

template<typename TData>
int Node<TData>::Hight()
{
    return m_hight;
}

template<typename TData>
int Node<TData>::Hight() const
{
    return m_hight;
}

template<typename TData>
int Node<TData>::Hight(int & set)
{
    if (m_hight == set) return m_hight; 
    m_hight = set;
    Write(ms_rwHight);
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
    return m_flag & 0x01;
}

template<typename TData>
void Node<TData>::Synchronize()
{
    Write();
}

template<typename TData>
TData & Node<TData>::operator*()
{
    return m_data;
}

template<typename TData>
const TData & Node<TData>::operator*() const
{
    return m_data;
}

template<typename TData>
bool Node<TData>::operator==(const NodeInterfaceType & other) const
{
    auto node = dynamic_cast<const Node<TData> *>(&other);
    return node && m_file == node->m_file &&
        m_position == node->m_position;
}

template<typename TData>
bool Node<TData>::operator!=(const NodeInterfaceType & other) const
{
    auto node = dynamic_cast<const Node<TData> *>(&other);
    return !node || m_file != node->m_file ||
        m_position != node->m_position;
}

template<typename TData>
Node<TData>::operator bool() const
{
    return m_file && m_position != -1 &&
        m_file->IsOpen();
}

} //file

} //!imp

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_IMP_FILE_NODE_H_