#include "mgmt/pathname/Cache.h"

#include <algorithm>
#include <utility>

using namespace std;
using namespace mgmt::pathname;

Cache::Cache() :
    m_maximum(-1),
    m_pathname_map(),
    m_key_queue()
{}

Cache::Cache(const std::size_t & max) :
    m_maximum(max),
    m_pathname_map(),
    m_key_queue()
{};

Cache::Cache(Cache && mov) :
    m_maximum(move(mov.m_maximum)),
    m_pathname_map(move(mov.m_pathname_map)),
    m_key_queue(move(mov.m_key_queue))
{}

void Cache::MapInsert(const KeyValueType & key, 
    const PathnameType & pathname)
{
    m_pathname_map.insert(std::make_pair(key, pathname));
}

typename Cache::MapIteratorType 
Cache::MapFind(const PathnameType & pathname)
{
    for (auto it = m_pathname_map.begin();
        it != m_pathname_map.end(); it++)
    {
        if (it->second == pathname)
            return it;
    }
    return m_pathname_map.end();
}

typename Cache::MapIteratorType 
Cache::MapFind(const KeyValueType & key)
{
    return m_pathname_map.find(key);
}

bool Cache::MapErase(MapIteratorType it_map)
{
    if (it_map == m_pathname_map.end()) return false;
    m_pathname_map.erase(it_map);
}

bool Cache::MapErase(const KeyValueType & key)
{
    if (key < 0 && key > m_pathname_map.size()) return false;
    m_pathname_map.erase(key);
}

bool Cache::MapHas(MapIteratorType it_map)
{
    return it_map != m_pathname_map.end();
}

void Cache::QueuePush(const KeyValueType & key)
{
    if (m_key_queue.size() == m_maximum &&
        m_maximum > 0)
    {
        MapErase(m_key_queue.back());
        m_key_queue.pop_back();
    }
    m_key_queue.push_front(key);
}

typename Cache::QueueIteratorType Cache::QueueFind(const KeyValueType & key)
{
    return std::find(m_key_queue.begin(), m_key_queue.end(), key);
}

bool Cache::QueueErase(QueueIteratorType it_queue)
{
    if (it_queue == m_key_queue.end()) return false;
    m_key_queue.erase(it_queue);
    return true;
}

bool Cache::QueueHas(QueueIteratorType it_queue)
{
    return it_queue != m_key_queue.end();
}

void Cache::Insert(const KeyValueType & key, 
    const PathnameType & pathname)
{
    if(!QueueErase(QueueFind(key)))
        MapInsert(key, pathname);
    QueuePush(key);
}

void Cache::Remove(const KeyValueType & key)
{
    if (QueueErase(QueueFind(key)))
        MapErase(key);
}

void Cache::Remove(const PathnameType & pathname)
{
    auto fn = MapFind(pathname);
    if (MapErase(fn))
        QueueErase(QueueFind(fn->first));
}

typename Cache::PathnameType 
Cache::Get(const KeyValueType & key)
{
    auto fn = MapFind(key);
    if (MapHas(fn)) return PathnameType();
    return fn->second;
}

typename Cache::KeyValueType 
Cache::Get(const PathnameType & pathname)
{
    auto fn = MapFind(pathname);
    if (MapHas(fn)) return -1;
    return fn->first;
}

bool Cache::Has(const KeyValueType & key)
{
    return QueueHas(QueueFind(key));
}

bool Cache::Has(const PathnameType & pathname)
{
    return MapHas(MapFind(pathname));
}
