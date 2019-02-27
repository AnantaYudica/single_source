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
    auto pathname_str = pathname.GetPathname();
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
    m_buff.pubsync();
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

bool Management::NextPathname(streampos & curr_pos)
{
    auto curr_offset = curr_pos % ms_end_offset;
    m_buff.pubseekoff(ms_end_offset - curr_offset, ios_base::cur, 
        ios_base::in);
    m_buff.in_avail() != EOF;
}

bool Management::NextKey(streampos & curr_pos)
{
    auto curr_offset = curr_pos % ms_end_offset;
    if (curr_offset <= ms_key_offset)
        curr_pos = m_buff.pubseekoff(ms_key_offset - curr_offset, 
            ios_base::cur, ios_base::in);
    else
        curr_pos = m_buff.pubseekoff((ms_end_offset - curr_offset) +
            ms_key_offset, ios_base::cur, ios_base::in);
    m_buff.in_avail() != EOF;
}

bool Management::Find(const Pathname & pathname)
{
    auto curr_pos = m_buff.pubseekpos(0, ios_base::in);
    streampos old_pos = curr_pos;
    do
    {
        if (GetPathname() == pathname) 
        {
            m_buff.pubseekpos(old_pos, ios_base::in);
            return true;
        }
        old_pos = curr_pos;
    } while(NextPathname(curr_pos));
    return false;
}

bool Management::Find(const KeyValueType & key)
{
    auto curr_pos = m_buff.pubseekpos(ms_key_offset, ios_base::in);
    streampos old_pos = curr_pos;
    do
    {
        if (GetKey() == key) 
        {
            m_buff.pubseekpos(old_pos, ios_base::in);
            return true;
        }
        old_pos = curr_pos;
    } while(NextKey(curr_pos));
    return false;
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

typename Management::KeyValueType 
Management::GetKey(Pathname pathname)
{
    lock_guard<std::mutex> lock{m_lock};
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
    if (Find(pathname))
        key = GetKey();
    else
    {
        key = m_size++;
        m_buff.pubseekoff(0, ios_base::end, ios_base::out);
        Push(pathname, key);
    }
    Update(key, pathname);
    return key;
}

Pathname Management::GetPathname(KeyValueType key)
{
    lock_guard<std::mutex> lock{m_lock};
    if (!Open()) return {};
    Pathname pathname;
    if (!Open()) return {};
    for(auto elem : m_pathname_map)
    {
        if (elem.first == key)
        {
            Update(elem.first);
            return elem.second;
        }
    }
    if (Find(key))
    {
        pathname = std::move(GetPathname());
        Update(key, pathname);
    }
    return std::move(pathname);
}

bool Management::Has(Pathname pathname)
{
    lock_guard<std::mutex> lock{m_lock};
    if (!Open()) return false;
    for(auto elem : m_pathname_map)
    {
        if (elem.second == pathname)
        {
            Update(elem.first);
            return true;
        }
    }
    if (Find(pathname))
    {
        Update(GetKey(), pathname);
        return true;
    }
    return false;
}

bool Management::Has(KeyValueType key)
{
    lock_guard<std::mutex> lock{m_lock};
    if (!Open()) return false;
    return key < m_size && key >= 0;
}
