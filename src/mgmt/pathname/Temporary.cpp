#include "mgmt/pathname/Temporary.h"
#include "file/imp/std/Filebuf.h"
#include "struc/tree/avl/trait/File.h"

#include <cstdio>
#include <utility>

using namespace std;
using namespace mgmt::pathname;

Temporary::Temporary() : 
    m_initial(false),
    m_linear_str(),
    m_tree_str(),
    m_linear_file(FileInterfacePointerType(new file::imp::std::
        Filebuf(ios_base::in | ios_base::out | ios_base::trunc))),
    m_tree_file(FileInterfacePointerType(new file::imp::std::
        Filebuf(ios_base::in | ios_base::out | ios_base::trunc))),
    m_linear_format_file(),
    m_tree_format_file(),
    m_tree_root_node(),
    m_tree(),
    m_compare_func(nullptr)
{
    m_initial = Initialize();
}

Temporary::Temporary(Temporary && mov) :
    m_initial(move(mov.m_initial)),
    m_linear_str(move(mov.m_linear_str)),
    m_tree_str(move(mov.m_tree_str)),
    m_linear_file(move(mov.m_linear_file)),
    m_tree_file(move(mov.m_tree_file)),
    m_linear_format_file(move(mov.m_linear_format_file)),
    m_tree_format_file(move(mov.m_tree_format_file)),
    m_tree_root_node(move(mov.m_tree_root_node)),
    m_tree(move(mov.m_tree)),
    m_compare_func(nullptr)
{
    mov.m_initial = false;
    mov.m_linear_str.clear();
    mov.m_tree_str.clear();
    mov.m_linear_file.reset();
    mov.m_tree_file.reset();
    m_compare_func = bind(&Temporary::Compare, this, placeholders::_1, 
        placeholders::_2);
}

Temporary::~Temporary()
{
    if (m_linear_file && m_linear_file->IsOpen())
    {
        m_linear_file->Close();
        remove(m_linear_str.c_str());
    }
    if (m_tree_file && m_tree_file->IsOpen())
    {
        m_tree_file->Close();
        remove(m_tree_str.c_str());
    }
}

bool Temporary::Initialize()
{
    if (m_linear_str.empty())
        m_linear_str = tmpnam(NULL);
    if (m_linear_str.empty()) return false;
    if (m_tree_str.empty())
        m_tree_str = tmpnam(NULL);
    if (m_tree_str.empty()) return false;
    if (!m_linear_file) return false;
    if (!m_linear_file->IsOpen())
    {
        if (!m_linear_file->Open(m_linear_str))
            return false;
        const size_t size = defn::rec::Size<LinearRecordType>::Value;
        m_linear_format_file = move(FileFormatLinearPointerType(
            new FileFormatLinearType(m_linear_file, size)));
    }
    if (!m_tree_file) return false;
    if (!m_tree_file->IsOpen())
    {
        if (!m_tree_file->Open(m_tree_str)) return false;
        const size_t size = defn::rec::Size<typename struc::tree::
            avl::imp::file::Node<TreeRecordType>::RecordType>::Value;
        m_tree_format_file = move(FileFormatLinearPointerType(
            new FileFormatLinearType(m_tree_file, size)));
        m_tree_root_node = move(TreeNodeType(m_tree_file, m_tree_format_file));
    }
    m_compare_func = bind(&Temporary::Compare, this, placeholders::_1, 
        placeholders::_2);
    return true;
}

int Temporary::Compare(const TreeRecordType & a_rec, 
    const TreeRecordType & b_rec)
{
    if (!m_initial) return -1;
    string a_pathname = a_rec.Pathname();
    string b_pathname = b_rec.Pathname();
    auto a_pos = a_rec.Position();
    auto b_pos = b_rec.Position();
    if (a_pathname.empty())
    {
        if (a_pos== -1) return -1;
        LinearRecordType linear_record;
        m_linear_format_file->SeekPosition(a_pos);
        m_linear_format_file->Get(linear_record);
        if (!linear_record.IsGood()) return -1;
        a_pathname = linear_record.Value();
    }
    if (b_pathname.empty())
    {
        if (b_pos== -1) return -1;
        LinearRecordType linear_record;
        m_linear_format_file->SeekPosition(b_pos);
        m_linear_format_file->Get(linear_record);
        if (!linear_record.IsGood()) return -1;
        b_pathname = linear_record.Value();
    }
    return a_pathname.compare(b_pathname);
}

typename Temporary::KeyValueType 
Temporary::Insert(const PathnameType & pathname)
{
    if (!m_initial) return -1;
    LinearRecordType linear_record;
    TreeRecordType tree_record;
    linear_record.Value(pathname.String());
    KeyValueType key = m_linear_format_file->Size();
    m_linear_format_file->SeekOffset(0, WayType::end);
    m_linear_format_file->Put(linear_record);
    tree_record.Position(key);
    tree_record.Pathname(pathname.String());
    m_tree.Insert<struc::tree::avl::trait::File>(m_tree_root_node, 
        tree_record, m_compare_func);
    return key;
}

typename Temporary::PathnameType Temporary::Remove(const KeyValueType & key)
{
    if (!m_initial && key != -1) return PathnameType();
    LinearRecordType linear_record;
    TreeRecordType tree_record;
    m_linear_format_file->SeekPosition(key);
    m_linear_format_file->Get(linear_record);
    if (!linear_record.IsGood()) return PathnameType();
    linear_record.Delete();
    m_linear_format_file->SeekPosition(key);
    m_linear_format_file->Put(linear_record);
    tree_record.Position(key);
    tree_record.Pathname(linear_record.Value());
    m_tree.Erase<struc::tree::avl::trait::File>(m_tree_root_node, 
        tree_record, m_compare_func);
    return PathnameType(linear_record.Value());
}

void Temporary::Remove(const PathnameType & pathname)
{
    if (!m_initial && !pathname) return;
    TreeRecordType tree_record;
    LinearRecordType linear_record;
    tree_record.Position(-1);
    tree_record.Pathname(pathname.String());
    tree_record = m_tree.Find(m_tree_root_node, tree_record, m_compare_func);
    if(!tree_record.IsGood() && tree_record.Position() != -1) return;
    m_linear_format_file->SeekPosition(tree_record.Position());
    m_linear_format_file->Get(linear_record);
    if (!linear_record.IsGood()) return;
    linear_record.Delete();
    m_linear_format_file->Put(linear_record);
    m_tree.Erase<struc::tree::avl::trait::File>(m_tree_root_node, 
        tree_record, m_compare_func);
}

typename Temporary::PathnameType 
Temporary::Get(const KeyValueType & key)
{
    if (!m_initial && key != -1) return PathnameType();
    LinearRecordType linear_record;
    TreeRecordType tree_record;
    m_linear_format_file->SeekPosition(key);
    m_linear_format_file->Get(linear_record);
    if (!linear_record.IsGood()) return PathnameType();
    if (linear_record.IsDelete()) return PathnameType();
    return PathnameType(linear_record.Value());
}

typename Temporary::KeyValueType 
Temporary::Get(const PathnameType & pathname)
{
    if (!m_initial && !pathname) return -1;
    TreeRecordType tree_record;
    tree_record.Position(-1);
    tree_record.Pathname(pathname.String());
    tree_record = m_tree.Find(m_tree_root_node, tree_record, m_compare_func);
    if(!tree_record.IsGood() && tree_record.Position() != -1) return -1;
    return tree_record.Position();
}

bool Temporary::Has(const KeyValueType & key)
{
    if (!m_initial && key != -1) return false;
    return static_cast<bool>(Get(key));
}

bool Temporary::Has(const PathnameType & pathname)
{
    if (!m_initial && !pathname) return false;
    return Get(pathname) != -1;
}
