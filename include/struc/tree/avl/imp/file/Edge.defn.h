#ifndef STRUC_TREE_AVL_IMP_FILE_EDGE_DEFN_H_
#define STRUC_TREE_AVL_IMP_FILE_EDGE_DEFN_H_

#include "Edge.decl.h"
#include "Node.decl.h"
#include "node/Point.h"
#include "../../defn/ptr/Way.h"
#include "../../../../../intf/File.h"

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
class Edge :
    public struc::tree::avl::intf::Edge<TData>
{
public:
    typedef defn::ptr::Way WayType;
public:
    typedef node::Point<TData> PointType;
public:
    typedef TData DataType;
    typedef struc::tree::avl::intf::Edge<DataType> EdgeInterfaceType;
    typedef struc::tree::avl::intf::Node<DataType> NodeInterfaceType; 
    typedef imp::file::Node<DataType> NodeType;
    typedef ::intf::File FileInterfaceType;
    typedef typename FileInterfaceType::PositionType PositionType;
private:
    template<typename ... TArgs>
    static void CreateInstance(NodeType ** node, TArgs && ... args);
    template<typename ... TArgs>
    static void CreateInstance(NodeType *const* node, 
        TArgs && ... args);
private:
    static void Reset(NodeType ** node);
    static void Reset(NodeType *const* node);
private:
    static void SetPosition(NodeType * point, const WayType & way,
        const PositionType & pos);
    static PositionType GetPosition(NodeType const* point, 
        const WayType & way);
private:
    NodeType * m_point;
    NodeType * m_next;
    WayType m_way;
public:
    Edge();
    Edge(const NodeType & point, WayType way);
public:
    ~Edge(); 
public:
    Edge(const Edge<TData> & cpy);
    Edge(Edge<TData> && mov);
public:
    Edge<TData> & operator=(const EdgeInterfaceType & cpy);
    Edge<TData> & operator=(EdgeInterfaceType && mov);
public:
    Edge<TData> & operator=(const NodeInterfaceType & node);
    Edge<TData> & operator=(NodeInterfaceType && node);
public:
    PositionType Position() const;
    void Position(const PositionType & pos);
public:
    void Synchronize(const bool & force);
    void Synchronize(const bool & force) const;
public:
    NodeInterfaceType & Get();
    const NodeInterfaceType & Get() const;
public:
    NodeInterfaceType & operator*();
    const NodeInterfaceType & operator*() const;
public:
    NodeInterfaceType * operator->();
    const NodeInterfaceType * operator->() const;
public:
    operator bool() const;
public:
    bool operator==(const EdgeInterfaceType & other) const;
    bool operator==(const NodeInterfaceType & other) const;
    bool operator!=(const EdgeInterfaceType & other) const;
    bool operator!=(const NodeInterfaceType & other) const;
};

} //file

} //!imp

} //!avl

} //!tree

} //!struc

#endif //!STRUC_TREE_AVL_IMP_FILE_EDGE_DEFN_H_
