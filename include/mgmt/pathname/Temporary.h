#ifndef MGMT_PATHNAME_TEMPORARY_H_
#define MGMT_PATHNAME_TEMPORARY_H_

#include "rec/Linear.h"
#include "rec/Tree.h"
#include "../../defn/mgmt/pathname/Key.h"
#include "../../defn/file/Mode.h"
#include "../../defn/file/Way.h"
#include "../../Pathname.h"
#include "../../intf/File.h"
#include "../../file/format/Linear.h"
#include "../../struc/tree/avl/intf/Node.h"
#include "../../struc/tree/avl/intf/Pointer.h"
#include "../../struc/tree/avl/imp/file/Pointer.h"
#include "../../struc/tree/avl/imp/file/Node.h"
#include "../../struc/tree/AVL.h"

#include <string>
#include <functional>

namespace mgmt
{
namespace pathname
{

class Temporary
{
public:
    typedef defn::mgmt::pathname::KeyValue KeyValueType;
    typedef ::Pathname PathnameType;
    typedef rec::Linear LinearRecordType;
    typedef rec::Tree TreeRecordType;
    typedef intf::File FileInterfaceType;
    typedef typename FileInterfaceType::PointerType
        FileInterfacePointerType;
    typedef defn::file::Mode ModeType;
    typedef defn::file::ModeValueType ModeValueType;
    typedef defn::file::Way WayType;
    typedef file::format::Linear FileFormatLinearType;
    typedef typename FileFormatLinearType::PointerType
        FileFormatLinearPointerType;
    typedef struc::tree::avl::intf::
        Pointer<TreeRecordType> TreePointerType;
    typedef struc::tree::avl::intf::
        Node<TreeRecordType> TreeNodeType;
    typedef typename TreeNodeType::PointerType 
        TreeNodePointerType;
    typedef struc::tree::AVL TreeType;
    using CompareFunctionType = int(const TreeRecordType &, 
        const TreeRecordType &);
private:
    bool m_initial;
    std::string m_linear_str;
    std::string m_tree_str;
    FileInterfacePointerType m_linear_file;
    FileInterfacePointerType m_tree_file;
    FileFormatLinearPointerType m_linear_format_file;
    FileFormatLinearPointerType m_tree_format_file;
    TreeNodePointerType m_tree_root_node;
    TreeType m_tree;
    std::function<CompareFunctionType> m_compare_func;
public:
    Temporary();
public:
    Temporary(const Temporary & cpy) = delete;
    Temporary(Temporary && mov);
public:
    ~Temporary();
public:
    Temporary & operator=(const Temporary & cpy) = delete;
    Temporary & operator=(Temporary && mov) = delete;
private:
    bool Initialize();
private:
    int Compare(const TreeRecordType & a_rec, 
        const TreeRecordType & b_rec);
public:
    KeyValueType Insert(const PathnameType & pathname);
public:
    PathnameType Remove(const KeyValueType & key);
    void Remove(const PathnameType & pathname);
public:
    PathnameType Get(const KeyValueType & key);
    KeyValueType Get(const PathnameType & pathname);
public:
    bool Has(const KeyValueType & key);
    bool Has(const PathnameType & pathname);

};

} //!pathname

} //!mgmt

#endif //!MGMT_PATHNAME_TEMPORARY_H_
