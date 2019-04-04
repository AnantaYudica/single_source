#include "mgmt/pathname/Temporary.h"
#include "file/imp/std/Filebuf.h"

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
    m_tree_root_node(move(mov.m_tree_root_node)),
    m_tree(move(mov.m_tree)),
    m_compare_func(nullptr)
{
    mov.m_initial = false;
    mov.m_linear_str.clear();
    mov.m_tree_str.clear();
    mov.m_linear_file.reset();
    mov.m_tree_file.reset();
    mov.m_tree_root_node.reset();
    m_compare_func = bind(&Temporary::Compare, this, placeholders::_1, 
        placeholders::_2);
}

Temporary::~Temporary()
{
    if (m_linear_file->IsOpen())
    {
        m_linear_file->Close();
        remove(m_linear_str.c_str());
    }
    if (m_tree_file->IsOpen())
    {
        m_tree_file->Close();
        remove(m_tree_str.c_str());
    }
}

bool Temporary::Initialize()
{
    if (!m_linear_str.empty())
        m_linear_str = tmpnam(NULL);
    if (m_linear_str.empty()) return false;
    if (!m_tree_str.empty())
        m_tree_str = tmpnam(NULL);
    if (m_tree_str.empty()) return false;
    if (!m_linear_file) return false;
    if (!m_linear_file->IsOpen())
    {
        if (!m_linear_file->Open(m_linear_str))
            return false;
        m_linear_format_file = move(FileFormatLinearType(m_linear_file, 
            static_cast<std::size_t>(defn::rec::Size<LinearRecordType>::
            Value)));
    }
    if (!m_tree_file) return false;
    if (!m_tree_file->IsOpen())
    {
        if (!m_tree_file->Open(m_tree_str)) return false;
        m_tree_root_node = move(TreeNodePointerType(new struc::tree::avl::
            imp::file::Node<TreeRecordType>(m_tree_file)));
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
        m_linear_format_file.SeekPosition(a_pos);
        m_linear_format_file.Get(linear_record);
        if (!linear_record.IsGood()) return -1;
        a_pathname = linear_record.Value();
    }
    if (b_pathname.empty())
    {
        if (b_pos== -1) return -1;
        LinearRecordType linear_record;
        m_linear_format_file.SeekPosition(b_pos);
        m_linear_format_file.Get(linear_record);
        if (!linear_record.IsGood()) return -1;
        b_pathname = linear_record.Value();
    }
    return a_pathname.compare(b_pathname);
}

void Temporary::Insert(const KeyValueType & key, 
    const PathnameType & pathname)
{
    if (!m_initial) return;
    LinearRecordType linear_record;
    TreeRecordType tree_record;
    linear_record.Value(pathname.String());
    m_linear_format_file.SeekOffset(0, WayType::end);
    m_linear_format_file.Put(linear_record);
    tree_record.Position(key);
    tree_record.Pathname(pathname.String());
    m_tree_root_node = m_tree.Insert(m_tree_root_node, tree_record,
        m_compare_func);
}

void Temporary::Remove(const KeyValueType & key)
{
    if (!m_initial && key != -1) return;
    LinearRecordType linear_record;
    TreeRecordType tree_record;
    m_linear_format_file.SeekPosition(key);
    m_linear_format_file.Get(linear_record);
    if (!linear_record.IsGood()) return;
    linear_record.Delete();
    m_linear_format_file.Put(linear_record);
    tree_record.Position(key);
    tree_record.Pathname(linear_record.Value());
    m_tree_root_node = m_tree.Erase(m_tree_root_node, tree_record,
        m_compare_func);
}

void Temporary::Remove(const PathnameType & pathname)
{
    if (!m_initial && !pathname) return;
    TreeRecordType tree_record;
    LinearRecordType linear_record;
    tree_record.Position(-1);
    tree_record.Pathname(pathname.String());
    auto node = m_tree.Find(m_tree_root_node, tree_record, m_compare_func);
    auto fn = **node;
    if(!*node && !fn.IsGood() && fn.Position() != -1) return;
    m_linear_format_file.SeekPosition(fn.Position());
    m_linear_format_file.Get(linear_record);
    if (!linear_record.IsGood()) return;
    linear_record.Delete();
    m_linear_format_file.Put(linear_record);
    m_tree_root_node = m_tree.Erase(m_tree_root_node, fn, m_compare_func);
}

typename Temporary::PathnameType 
Temporary::Get(const KeyValueType & key)
{
    if (!m_initial && key != -1) return PathnameType();
    LinearRecordType linear_record;
    TreeRecordType tree_record;
    m_linear_format_file.SeekPosition(key);
    m_linear_format_file.Get(linear_record);
    if (!linear_record.IsGood()) return PathnameType();
    return PathnameType(linear_record.Value());
}

typename Temporary::KeyValueType 
Temporary::Get(const PathnameType & pathname)
{
    if (!m_initial && !pathname) return -1;
    TreeRecordType tree_record;
    tree_record.Position(-1);
    tree_record.Pathname(pathname.String());
    auto node = m_tree.Find(m_tree_root_node, tree_record, m_compare_func);
    auto fn = **node;
    if(!*node && !fn.IsGood() && fn.Position() != -1) return -1;
    return fn.Position();
}

bool Temporary::Has(const KeyValueType & key)
{
    if (!m_initial && key != -1) return false;
    LinearRecordType linear_record;
    m_linear_format_file.SeekPosition(key);
    m_linear_format_file.Get(linear_record);
    if (!linear_record.IsGood()) return false;
    return !linear_record.IsDelete();
}

bool Temporary::Has(const PathnameType & pathname)
{
    if (!m_initial && !pathname) return false;
    TreeRecordType tree_record;
    tree_record.Position(-1);
    tree_record.Pathname(pathname.String());
    auto node = m_tree.Find(m_tree_root_node, tree_record, m_compare_func);
    auto fn = **node;
    if(!*node && !fn.IsGood() && fn.Position() != -1) return false;
    return Has(fn.Position());
}
