#include "pathname/Management.h"
#include "Configure.h"

#include <cstdio>
#include <cstdlib>
#include <utility>
#include <iostream>

using namespace std;
using namespace pathname;

Management Management::ms_instance;

Management & Management::GetInstance()
{
    return ms_instance;
}

Management::Management() :
    m_size{0}
{}

Management::~Management()
{
    if (m_buff.is_open())
    {
        m_buff.close();
        remove(m_pathname_str.c_str());
    }
}

bool Management::Open()
{
    if (m_buff.is_open())
        return true;
    m_pathname_str = tmpnam(NULL);
    return m_buff.open(m_pathname_str.c_str(), ios_base::out | 
        ios_base::in | ios_base::trunc) != NULL;
}

void Management::Push(const Pathname & pathname, 
        const KeyValueType & key)
{
    char key_cstr[ms_key_alloc_size];
    auto key_size = snprintf(key_cstr, ms_key_alloc_size, "%zu", key);
    auto pathname_str = pathname.String();
    for (std::size_t i = 0; i < ms_pathname_alloc_size; ++i)
    {
        if (i < pathname_str.size())
            m_buff.sputc(pathname_str[i]);
        else
            m_buff.sputc(ms_empty_ch);
    }
    m_buff.sputc(ms_conj_ch);
    for (std::size_t i = 0; i < ms_key_alloc_size; ++i)
    {
        if (i < key_size)
            m_buff.sputc(key_cstr[i]);
        else
            m_buff.sputc(ms_empty_ch);
    }
    m_buff.sputc(ms_end_ch);
}

Pathname Management::GetPathname()
{
    char buff_cstr[ms_pathname_alloc_size];
    char pathname_cstr[ms_pathname_alloc_size];
    auto size = m_buff.sgetn(buff_cstr, ms_pathname_alloc_size);
    for (std::size_t i = 0; i < ms_pathname_alloc_size; ++i)
    {
        if (buff_cstr[i] == ms_conj_ch ||
            buff_cstr[i] == ms_empty_ch)
        {
            pathname_cstr[i] = '\0';
            break; 
        }
        pathname_cstr[i] = buff_cstr[i];
    }
    return {pathname_cstr};
}

typename Management::KeyValueType Management::GetKey()
{
    char buff_cstr[ms_key_alloc_size];
    char key_cstr[ms_key_alloc_size];
    auto size = m_buff.sgetn(buff_cstr, ms_key_alloc_size);
    for (std::size_t i = 0; i < ms_pathname_alloc_size; ++i)
    {
        if (buff_cstr[i] == ms_end_ch ||
            buff_cstr[i] == ms_empty_ch)
        {
            key_cstr[i] = '\0';
            break; 
        }
        key_cstr[i] = buff_cstr[i];
    }
    return strtoull(key_cstr, NULL, 10);
}

void Management::Back(std::streampos & curr_pos)
{
    auto curr_offset = curr_pos % ms_end_offset;
    curr_pos -= curr_offset;
}

void Management::Back(const std::ios_base::openmode & mode)
{
    auto curr_pos = m_buff.pubseekoff(0, ios_base::cur, mode);
    Back(curr_pos);
    m_buff.pubseekpos(curr_pos, mode);
}

bool Management::NextPathname(const std::ios_base::openmode & mode)
{
    auto curr_pos = m_buff.pubseekoff(0, ios_base::cur, mode);
    auto curr_offset = curr_pos % ms_end_offset;
    m_buff.pubseekoff(ms_end_offset - curr_offset, ios_base::cur, 
        mode);
    m_buff.in_avail() != EOF;
}

bool Management::NextKey(const std::ios_base::openmode & mode)
{
    auto curr_pos = m_buff.pubseekoff(0, ios_base::cur, mode);
    auto curr_offset = curr_pos % ms_end_offset;
    if (curr_offset <= ms_key_offset)
        curr_pos = m_buff.pubseekoff(ms_key_offset - curr_offset, 
            ios_base::cur, mode);
    else
        curr_pos = m_buff.pubseekoff((ms_end_offset - curr_offset) +
            ms_key_offset, ios_base::cur, mode);
    m_buff.in_avail() != EOF;
}

bool Management::Find(const Pathname & pathname, 
    const std::ios_base::openmode & mode)
{
    auto curr_pos = m_buff.pubseekpos(0, mode);
    streampos old_pos = curr_pos;
    do
    {
        if (GetPathname() == pathname) 
        {
            m_buff.pubseekpos(old_pos, mode);
            return true;
        }
        old_pos = m_buff.pubseekoff(0, ios_base::cur, mode);
    } while(NextPathname(mode));
    return false;
}

bool Management::Find(const KeyValueType & key, 
    const std::ios_base::openmode & mode)
{
    if (key >= m_size) return false;
    m_buff.pubseekpos(key * ms_end_offset, mode);
    return GetPathname();
}

bool Management::Find(const KeyValueType & key, Pathname & pathname, 
    const std::ios_base::openmode & mode)
{
    if (key >= m_size) return false;
    m_buff.pubseekpos(key * ms_end_offset, mode);
    return (pathname = std::move(GetPathname()));
}

void Management::Update(const KeyValueType & key)
{
    auto it = find(m_key_queue.begin(), m_key_queue.end(), key);
    if (it == m_key_queue.begin()) return;
    auto key_queue = *it;
    m_key_queue.erase(it);
    m_key_queue.push_front(key_queue);
}

void Management::Update(const KeyValueType & key, const Pathname & pathname)
{
    const auto queue_size = Configure::GetInstance().
        GetPathnameManagementQueueSize();
    if(m_pathname_map.size() >= queue_size)
    {
        m_pathname_map.erase(m_key_queue.back());
        m_key_queue.pop_back();
    }
    m_key_queue.push_front(key);
    m_pathname_map.insert(pair<KeyValueType, Pathname>{key, pathname});
}

void Management::Erase()
{
    for (size_t i = 0; i < ms_pathname_alloc_size; ++i)
        m_buff.sputc(ms_empty_ch);
}

typename Management::KeyValueType 
Management::Register(Pathname pathname)
{
    lock_guard<mutex> lock{m_lock};
    if (!Open()) return -1;
    for(auto elem : m_pathname_map)
    {
        if (elem.second == pathname)
        {
            Update(elem.first);
            return elem.first;
        }
    }
    KeyValueType key = -1;
    if (Find(pathname, ios_base::in))
    {
        NextKey(ios_base::in);
        key = GetKey();
    }
    else
    {
        key = m_size++;
        m_buff.pubseekoff(0, ios_base::end, ios_base::out);
        Push(pathname, key);
    }
    Update(key, pathname);
    return key;
}

Pathname Management::Unregister(KeyValueType key)
{
    lock_guard<mutex> lock{m_lock};
    if (!Open()) return {};
    Pathname pathname;
    auto it_map = m_pathname_map.find(key);
    if (it_map != m_pathname_map.end())
        m_pathname_map.erase(it_map);
    if (Find(key, pathname, ios_base::out | ios_base::in))
    {
        Back(ios_base::out);
        Erase();
    }
    return pathname;
}

void Management::Unregister(Pathname pathname)
{
    lock_guard<mutex> lock{m_lock};
    if (!Open()) return;
    KeyValueType key = -1;
    for(auto elem : m_pathname_map)
    {
        if (elem.second == pathname)
        {
            key = elem.first;
            break;
        }
    }
    if (key != -1)
    {
        m_pathname_map.erase(key);
        Find(key, ios_base::out);
        Back(ios_base::out);
        Erase();
    }
    else if (Find(pathname, ios_base::in | ios_base::out))
    {
        Back(ios_base::out);
        Erase();
    }
}

Pathname Management::Get(KeyValueType key)
{
    lock_guard<mutex> lock{m_lock};
    if (!Open()) return {};
    Pathname pathname;
    for(auto elem : m_pathname_map)
    {
        if (elem.first == key)
        {
            Update(elem.first);
            return elem.second;
        }
    }
    if (Find(key, pathname, ios_base::in))
        Update(key, pathname);
    return move(pathname);
}

typename Management::KeyValueType 
Management::Get(Pathname pathname)
{
    lock_guard<mutex> lock{m_lock};
    if (!Open()) return {};
    for(auto elem : m_pathname_map)
    {
        if (elem.second == pathname)
        {
            Update(elem.first);
            return elem.first;
        }
    }
    if (Find(pathname, ios_base::in))
    {
        Back(ios_base::in);
        NextKey(ios_base::in);
        Update(GetKey(), pathname);
    }
    return move(pathname);
}

bool Management::Has(Pathname pathname)
{
    lock_guard<mutex> lock{m_lock};
    if (!Open()) return false;
    for(auto elem : m_pathname_map)
    {
        if (elem.second == pathname)
        {
            Update(elem.first);
            return true;
        }
    }
    if (Find(pathname, ios_base::in))
    {
        NextKey(ios_base::in);
        Update(GetKey(), pathname);
        return true;
    }
    return false;
}

bool Management::Has(KeyValueType key)
{
    lock_guard<mutex> lock{m_lock};
    if (!Open()) return false;
    return Find(key, ios_base::in);
}
