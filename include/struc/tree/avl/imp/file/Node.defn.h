#ifndef STRUC_TREE_AVL_IMP_FILE_NODE_DEFN_H_
#define STRUC_TREE_AVL_IMP_FILE_NODE_DEFN_H_

#include "Node.decl.h"
#include "Edge.defn.h"
#include "node/Record.h"
#include "node/Point.h"
#include "../../../../../file/format/Linear.h"
#include "../../../../../intf/File.h"

#include <cstddef>
#include <cstdint>

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
class Node :
    public struc::tree::avl::intf::Node<TData>
{
public:
    typedef node::Record<TData> RecordType;
    typedef typename RecordType::FlagsValueType FlagsValueType;
    typedef typename RecordType::HightValueType HightValueType;
public:
    typedef node::Point<TData> PointType;
public:
    typedef ::intf::File FileInterfaceType;
    typedef typename FileInterfaceType::PointerType 
        FileInterfacePointerType;
private:
    typedef ::file::format::Linear FileFormatLinearType;
    typedef typename FileFormatLinearType::PointerType
        FileFormatLinearPointerType;
public:
    typedef typename FileInterfaceType::PositionType PositionType;
    typedef typename FileInterfaceType::OffsetType OffsetType;
    typedef typename FileInterfaceType::SizeType SizeType;
public:
    typedef typename FileInterfaceType::WayType WayType;
    typedef typename FileInterfaceType::ModeType ModeType;
    typedef typename FileInterfaceType::ModeValueType ModeValueType;
public:
    typedef struc::tree::avl::intf::Node<TData> NodeInterfaceType;
public: 
    typedef struc::tree::avl::intf::Edge<TData> EdgeInterfaceType;
    typedef typename Edge<TData>::WayType EdgeWayType;
public:
    typedef TData DataType;
private:
    template<typename ... TArgs>
    static void CreateInstance(Edge<TData> ** edge, TArgs && ... args);
    template<typename ... TArgs>
    static void CreateInstance(Edge<TData> *const* edge, TArgs && ... args);
    template<typename ... TArgs>
    static void CreateInstance(PointType ** point, TArgs && ... args);
    template<typename ... TArgs>
    static void CreateInstance(PointType *const* point, TArgs && ... args);
private:
    static void Reset(Edge<TData> ** edge);
    static void Reset(Edge<TData> *const* edge);
    static void Reset(PointType ** point);
    static void Reset(PointType *const* point);
private:
    PointType * m_point;
    Edge<TData> * m_parent;
    Edge<TData> * m_right;
    Edge<TData> * m_left;
    FileInterfacePointerType m_file;
    FileFormatLinearPointerType m_fileFormatLinear;
public:
    Node();
    Node(FileInterfacePointerType file, FileFormatLinearPointerType format);
    Node(FileInterfacePointerType file, FileFormatLinearPointerType format, 
        const PositionType & position);
public:
    ~Node(); 
public:
    Node(const Node<TData> & cpy);
    Node(Node<TData> && mov);
public:
    Node<TData> & operator=(const Node<TData> & cpy);
    Node<TData> & operator=(Node<TData> && mov);
    Node<TData> & operator=(const NodeInterfaceType & cpy);
    Node<TData> & operator=(NodeInterfaceType && mov);
private:
    bool IsValid() const;
public:
    Node<TData> Instance(PositionType position = -1) const;
public:
    PositionType Allocate();
public:
    PositionType Position() const;
public:
    bool HasParent() const;
public:
    EdgeInterfaceType & Parent();
    const EdgeInterfaceType & Parent() const;
public:
    PositionType ParentPosition() const;
    void ParentPosition(const PositionType & new_position);
public:
    bool HasLeft() const;
public:
    EdgeInterfaceType & Left();
    const EdgeInterfaceType & Left() const;
public:
    PositionType LeftPosition() const;
    void LeftPosition(const PositionType & new_position);
public:
    bool HasRight() const;
public:
    EdgeInterfaceType & Right();
    const EdgeInterfaceType & Right() const;
public:
    PositionType RightPosition() const;
    void RightPosition(const PositionType & new_position);
public:
    HightValueType Hight() const;
    void Hight(const HightValueType & set);
public:
    bool IsDeleted() const;
public:
    void Delete();
public:
    TData Data() const;
    void Data(const TData & data);
public:
    int Balance() const;
public:
    void Synchronize(const bool & force);
    void Synchronize(const bool & force) const;
public:
    void Update();
public:
    bool operator==(const NodeInterfaceType & other) const;
    bool operator!=(const NodeInterfaceType & other) const;
public:
    operator bool() const;
};

} //file

} //!imp

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_IMP_FILE_NODE_DEFN_H_
