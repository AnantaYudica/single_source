#ifndef STRUC_TREE_AVL_IMP_FILE_NODE_POINT_DEFN_H_
#define STRUC_TREE_AVL_IMP_FILE_NODE_POINT_DEFN_H_

#include "Record.h"
#include "../../../../../../intf/File.h"

#include <cstddef>

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
class Point
{
public:
    typedef node::Record<TData> RecordType;
public:
    typedef ::intf::File FileInterfaceType;
    typedef typename FileInterfaceType::PositionType PositionType;
private:
    PositionType m_position;
    RecordType m_record;
public:
    Point(const PositionType & pos);
    ~Point();
public:
    Point(const Point<TData> & cpy);
    Point(Point<TData> && mov);
public:
    Point<TData> & operator=(const Point<TData> & cpy);
    Point<TData> & operator=(Point<TData> && mov);
public:
    PositionType Position() const;
public:
    RecordType & Record();
};

} //!node

} //!file

} //!imp

} //!avl

} //!tree

} //!struc


#endif //!STRUC_TREE_AVL_IMP_FILE_NODE_POINT_DEFN_H_
