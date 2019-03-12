#ifndef FILE_FORMAT_TREE_AVL_NODE_H_
#define FILE_FORMAT_TREE_AVL_NODE_H_

#include "struc/tree/avl/intf/Node.h"
#include "struc/tree/avl/intf/Pointer.h"
#include "Pointer.h"

#include <fstream>
#include <string>
#include <utility>
#include <cstdint>
#include <cstring>

namespace file
{
namespace format
{
namespace tree
{
namespace avl
{

template<typename TData>
class Node :
    public struc::tree::avl::intf::Node<TData>
{
public:
    typedef struc::tree::avl::intf::Pointer<Node<TData>> PointerInterfaceType;
    typedef Pointer<Node<TData>> PointerType;
public:
    typedef TData DataType;
public:
    typedef std::uint8_t FlagValueType;
    typedef std::uint32_t HightValueType;
    typedef std::uint64_t PositionType;
private:
    typedef std::uint8_t FlagReadWrite;
private:
    constexpr static std::size_t ms_flagOffset = 0;
    constexpr static std::size_t ms_hightOffset = sizeof(FlagValueType);
    constexpr static std::size_t ms_parentPositionOffset =
        ms_hightOffset + sizeof(HightValueType);
    constexpr static std::size_t ms_rightPositionOffset =
        ms_parentPositionOffset + sizeof(PositionType);
    constexpr static std::size_t ms_leftPositionOffset =
        ms_rightPositionOffset + sizeof(PositionType);
    constexpr static std::size_t ms_dataOffset =
        ms_leftPositionOffset + sizeof(PositionType);
    constexpr static std::size_t ms_endOffset = 
        ms_dataOffset + sizeof(TData);
private:
    constexpr static FlagReadWrite ms_rwFlag = 0x01;
    constexpr static FlagReadWrite ms_rwHight = 0x02;
    constexpr static FlagReadWrite ms_rwParentPosition = 0x04;
    constexpr static FlagReadWrite ms_rwRightPosition = 0x08;
    constexpr static FlagReadWrite ms_rwLeftPosition = 0x10;
    constexpr static FlagReadWrite ms_rwData = 0x20;
private:
    std::streampos m_position;
    FlagValueType m_flag;
    HightValueType m_hight;
    PositionType m_parentPosition;
    PositionType m_rightPosition;
    PositionType m_leftPosition;
    std::filebuf * m_filebuffer;
    PointerInterfaceType * m_Parent;
    PointerInterfaceType * m_right;
    PointerInterfaceType * m_left;
    TData m_data;
public:
    inline Node();
    inline Node(std::filebuf * filebuffer);
    inline Node(std::filebuf * filebuffer, std::streampos position);
public:
    inline ~Node(); 
public:
    inline Node(const Node<TData> & cpy);
    inline Node(Node<TData> && mov);
public:
    inline Node<TData> & operator=(const Node<TData> & cpy);
    inline Node<TData> & operator=(Node<TData> && mov);
private:
    inline void Default();
private:
    template<typename TValue>
    inline TValue ReadValue(std::streampos position);
    template<typename TValue>
    inline void WriteValue(const TValue & val, std::streampos position);
private:
    inline void Read(std::uint8_t flags = 0xFF);
    inline void Write(std::uint8_t flags = 0xFF);
public:
    inline Node<TData> & Emplace(TData & data);
public:
    inline Node<TData> & Displace();
public:
    inline PointerInterfaceType & Parent();
    inline void Parent(const PositionType & new_position);
public:
    inline PointerInterfaceType & Right();
    inline void Right(const PositionType & new_position);
public:
    inline PointerInterfaceType & Left();
    inline void Left(const PositionType & new_position);
public:
    inline int Hight();
    inline int Hight(int & set);
    inline int Balance();
    inline TData Value();
    inline void Swap(Node<TData> & node);
public:
    inline bool IsDeleted();
public:
    inline bool operator==(Node<TData> other) const;
    inline bool operator!=(Node<TData> other) const;
public:
    inline operator bool();
};

template<typename TData>
inline Node<TData>::Node() :
    m_filebuffer(nullptr),
    m_position(-1),
    m_flag(0),
    m_hight(0),
    m_parentPosition(-1),
    m_rightPosition(-1),
    m_leftPosition(-1),
    m_Parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{}

template<typename TData>
inline Node<TData>::Node(std::filebuf * filebuffer) :
    m_filebuffer(filebuffer),
    m_position(-1),
    m_flag(0),
    m_hight(0),
    m_parentPosition(-1),
    m_rightPosition(-1),
    m_leftPosition(-1),
    m_Parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{}

template<typename TData>
inline Node<TData>::Node(std::filebuf * filebuffer, std::streampos position) :
    m_filebuffer(filebuffer),
    m_position(position),
    m_flag(0),
    m_hight(0),
    m_parentPosition(-1),
    m_rightPosition(-1),
    m_leftPosition(-1),
    m_Parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{
    Read();
    m_Parent = new PointerType(*this, PointerType::Way::parent);
    m_right = new PointerType(*this, PointerType::Way::right);
    m_left = new PointerType(*this, PointerType::Way::left);
}

template<typename TData>
inline Node<TData>::~Node()
{
    if (m_Parent) delete m_Parent;
    if (m_right) delete m_right;
    if (m_left) delete m_left;
}

template<typename TData>
inline Node<TData>::Node(const Node<TData> & cpy) :
    m_filebuffer(cpy.m_filebuffer),
    m_position(cpy.m_position),
    m_flag(cpy.m_flag),
    m_hight(cpy.m_hight),
    m_parentPosition(cpy.m_parentPosition),
    m_rightPosition(cpy.m_rightPosition),
    m_leftPosition(cpy.m_leftPosition),
    m_Parent(nullptr),
    m_right(nullptr),
    m_left(nullptr)
{
    m_Parent = new PointerType(*this, PointerType::Way::parent);
    m_right = new PointerType(*this, PointerType::Way::right);
    m_left = new PointerType(*this, PointerType::Way::left);
}

template<typename TData>
inline Node<TData>::Node(Node<TData> && mov) :
    m_filebuffer(mov.m_filebuffer),
    m_position(mov.m_position),
    m_flag(mov.m_flag),
    m_hight(mov.m_hight),
    m_parentPosition(mov.m_parentPosition),
    m_rightPosition(mov.m_rightPosition),
    m_leftPosition(mov.m_leftPosition),
    m_Parent(mov.m_Parent),
    m_right(mov.m_right),
    m_left(mov.m_left)
{
    mov.Default();
}

template<typename TData>
inline Node<TData> & Node<TData>::operator=(const Node<TData> & cpy)
{
    m_filebuffer = cpy.m_filebuffer;
    m_position = cpy.m_position;
    m_flag = cpy.m_flag;
    m_hight = cpy.m_hight;
    m_parentPosition = cpy.m_parentPosition;
    m_rightPosition = cpy.m_rightPosition;
    m_leftPosition = cpy.m_leftPosition;
    m_Parent = new PointerType(*this, PointerType::Way::parent);
    m_right = new PointerType(*this, PointerType::Way::right);
    m_left = new PointerType(*this, PointerType::Way::left);
    return *this;
}

template<typename TData>
inline Node<TData> & Node<TData>::operator=(Node<TData> && mov)
{
    m_filebuffer = mov.m_filebuffer);
    m_position = mov.m_position);
    m_flag = mov.m_flag;
    m_hight = mov.m_hight;
    m_parentPosition = mov.m_parentPosition;
    m_rightPosition = mov.m_rightPosition;
    m_leftPosition = mov.m_leftPosition;
    m_Parent = mov.m_Parent;
    m_right = mov.m_right;
    m_left = mov.m_left;

    mov.Default();
    return *this;
}

template<typename TData>
inline void Node<TData>::Default()
{
    m_filebuffer = nullptr;
    m_position = -1;
    m_flag = 0;
    m_hight = 0;
    m_parentPosition = -1;
    m_rightPosition = -1;
    m_leftPosition = -1;
    m_Parent = nullptr;
    m_right = nullptr;
    m_left = nullptr;
}

template<typename TData>
template<typename TValue>
inline TValue Node<TData>::ReadValue(std::streampos position)
{
    char buff[sizeof(TValue)];
    TValue value;
    m_filebuffer->pubseekpos(position);
    m_filebuffer->sgetn(buff, sizeof(buff));
    value = *reinterpret_cast<TValue *>(buff);
    return value;
}

template<typename TData>
template<typename TValue>
inline void Node<TData>::WriteValue(const TValue & val, 
    std::streampos position)
{
    char buff[sizeof(TValue)];
    memccpy(buff, (const char *)&val, sizeof(TValue));
    m_filebuffer->pubseekpos(position);
    m_filebuffer->sputn(buff, sizeof(buff));
}

template<typename TData>
inline void Node<TData>::Read(std::uint8_t flags)
{
    if (flags & ms_rwFlag)
        m_flag = ReadValue<FlagType>(m_position + ms_flagOffset);
    if (flags & ms_rwHight)
        m_hight = ReadValue<HightValueType>(m_position + ms_hightOffset);
    if (flags & ms_rwParentPosition)
        m_parentPosition = ReadValue<PositionType>(m_position + 
            ms_parentPositionOffset);
    if (flags & ms_rwRightPosition)
        m_rightPosition = ReadValue<PositionType>(m_position + 
            ms_rightPositionOffset);
    if (flag & ms_rwLeftPosition)
        m_leftPosition = ReadValue<PositionType>(m_position + 
            ms_leftPositionOffset);
    if (flag & ms_rwData)
        m_data = ReadValue<DataType>(m_position + ms_dataOffset);
}

template<typename TData>
inline void Node<TData>::Write(std::uint8_t flags)
{
    if (flags & ms_rwFlag)
        WriteValue<FlagType>(m_flag, m_position + ms_flagOffset);
    if (flags & ms_rwHight)
        WriteValue<HightValueType>(m_hight, m_position + ms_hightOffset);
    if (flags & ms_rwParentPosition)
        WriteValue<PositionType>(m_parentPosition, m_position + 
            ms_parentPositionOffset);
    if (flags & ms_rwRightPosition)
        WriteValue<PositionType>(m_rightPosition, m_position + 
            ms_rightPositionOffset);
    if (flag & ms_rwLeftPosition)
        WriteValue<PositionType>(m_leftPosition, m_position + 
            ms_leftPositionOffset);
    if (flag & ms_rwData)
        WriteValue<DataType>(m_data, m_position + ms_dataOffset);
}

template<typename TData>
inline Node<TData> & Node<TData>::Emplace(TData & data)
{
    if (!m_filebuffer || !m_filebuffer->is_open()) return *this;
    m_flag = 0;
    m_hight = 0;
    m_parentPosition = -1;
    m_rightPosition = -1;
    m_leftPosition = -1;
    m_data = data;
    if (m_position == -1)
        m_position = m_filebuffer->pubseekoff(0, std::ios_base::end);
    Write();
    return *this;
}

template<typename TData>
inline Node<TData> & Node<TData>::Displace()
{
    if (!m_filebuffer || !m_filebuffer->is_open()) return *this;
    m_flag |= 0x01;
    Write(0x01);
    return *this;
}

template<typename TData>
inline typename Node<TData>::PointerInterfaceType & 
Node<TData>::Parent()
{
    if (!m_parent) 
        return *(m_Parent = new PointerType(*this, PointerType::Way::parent));
    return *m_parent;
}

template<typename TData>
inline void Node<TData>::Parent(const PositionType & new_position)
{
    if (m_parentPosition == new_position) return;
    m_parentPosition = new_position;
    Write(ms_rwParentPosition);
}

template<typename TData>
inline typename Node<TData>::PointerInterfaceType & 
Node<TData>::Right()
{
    if (!m_right) 
        return *(m_right = new PointerType(*this, PointerType::Way::right));
    return *m_right;
}

template<typename TData>
inline void Node<TData>::Right(const PositionType & new_position)
{
    if (m_rightPosition == new_position) return;
    m_rightPosition = new_position;
    Write(ms_rwRightPosition);
}

template<typename TData>
inline typename Node<TData>::PointerInterfaceType & 
Node<TData>::Left()
{
    if (!m_left) 
        return *(m_left = new PointerType(*this, PointerType::Way::left));
    return *m_left;
}

template<typename TData>
inline void Node<TData>::Left(const PositionType & new_position)
{
    if (m_leftPosition == new_position) return;
    m_leftPosition = new_position;
    Write(ms_rwLeftPosition);
}

template<typename TData>
inline int Node<TData>::Hight()
{
    return m_hight;
}

template<typename TData>
inline int Node<TData>::Hight(int & set)
{
    if (m_hight == set) return m_hight; 
    m_hight = set;
    Write(ms_rwHight);
}

template<typename TData>
inline int Node<TData>::Balance()
{
    return Right()->Hight() - Left()->Left();
}

template<typename TData>
inline TData Node<TData>::Value()
{
    return m_data;
}

template<typename TData>
inline void Node<TData>::Swap(Node<TData> & node)
{
    Node<TData> cpy{node};
    *this = std::move(node);
    node = std::move(cpy);
}

template<typename TData>
inline bool Node<TData>::operator==(Node<TData> other) const
{
    return m_filebuffer == other.m_filebuffer &&
        m_position == other.m_position;
}

template<typename TData>
inline bool Node<TData>::operator!=(Node<TData> other) const
{
    return m_filebuffer != other.m_filebuffer ||
        m_position != other.m_position;
}

template<typename TData>
inline Node<TData>::operator bool()
{
    return m_filebuffer && m_position != -1;
}

} //!avl

} //!tree

} //!format

} //!file

#endif //!FILE_FORMAT_TREE_AVL_NODE_H_