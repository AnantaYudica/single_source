#ifndef MGMT_PATHNAME_CACHE_H_
#define MGMT_PATHNAME_CACHE_H_

#include "../../defn/mgmt/pathname/Key.h"
#include "../../Pathname.h"

#include <deque>
#include <map>
#include <cstddef>

namespace mgmt
{
namespace pathname
{

class Cache
{
public:
    typedef std::uint8_t FlagsType;
    typedef defn::mgmt::pathname::KeyValue KeyValueType;
    typedef ::Pathname PathnameType;
private:
    typedef std::map<KeyValueType, PathnameType> MapType;
    typedef typename MapType::iterator MapIteratorType;
    typedef std::deque<KeyValueType> QueueType;
    typedef typename QueueType::iterator QueueIteratorType;
private:
    std::size_t m_maximum;
    MapType m_pathname_map;
    QueueType m_key_queue;
public:
    Cache();
    Cache(const std::size_t & max);
public:
    Cache(const Cache & cpy) = delete;
    Cache(Cache && mov);
public:
    Cache & operator=(const Cache & cpy) = delete;
    Cache & operator=(Cache && mov) = delete;
private:
    void MapInsert(const KeyValueType & key, 
        const PathnameType & pathname);
private:
    MapIteratorType MapFind(const KeyValueType & key);
    MapIteratorType MapFind(const PathnameType & pathname);
private:
    bool MapErase(MapIteratorType it_map);
    bool MapErase(const KeyValueType & key);
private:
    bool MapHas(MapIteratorType it);
private:
    void QueuePush(const KeyValueType & key);
private:
    QueueIteratorType QueueFind(const KeyValueType & key);
private:
    bool QueueErase(QueueIteratorType it_queue);
private:
    bool QueueHas(QueueIteratorType it_queue);
public:
    void Insert(const KeyValueType & key, 
        const PathnameType & pathname);
public:
    void Remove(const KeyValueType & key);
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

#endif //!MGMT_PATHNAME_CACHE_H_
