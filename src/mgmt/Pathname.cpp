#include "mgmt/Pathname.h"

using namespace std;

namespace mgmt
{

Pathname Pathname::ms_instance;

Pathname & Pathname::GetInstance()
{
    return ms_instance;
}

Pathname::Pathname() :
    m_cache(20),
    m_temporary()
{}

Pathname::~Pathname()
{}

typename Pathname::KeyValueType 
Pathname::Register(const PathnameType & pathname)
{
    lock_guard<mutex> lock(m_mutex);
    KeyValueType key = m_temporary.Insert(pathname);
    m_cache.Insert(key, pathname);
}

typename Pathname::PathnameType 
Pathname::Unregister(const KeyValueType & key)
{
    lock_guard<mutex> lock(m_mutex);
    m_cache.Remove(key);
    return m_temporary.Remove(key);
}

void Pathname::Unregister(const PathnameType & pathname)
{
    lock_guard<mutex> lock(m_mutex);
    m_cache.Remove(m_temporary.Get(pathname));
    m_temporary.Remove(pathname);
}

typename Pathname::PathnameType Pathname::Get(const KeyValueType & key)
{
    lock_guard<mutex> lock(m_mutex);
    auto get = m_cache.Get(key);
    if (get) return get;
    return m_temporary.Get(key);
}

typename Pathname::KeyValueType Pathname::Get(const PathnameType & pathname)
{
    lock_guard<mutex> lock(m_mutex);
    return m_temporary.Get(pathname);
}

bool Pathname::Has(const KeyValueType & key)
{
    lock_guard<mutex> lock(m_mutex);
    return m_cache.Has(key) ||
        m_temporary.Has(key);
}

bool Pathname::Has(const PathnameType & pathname)
{
    lock_guard<mutex> lock(m_mutex);
    return m_temporary.Get(pathname);
}

} //!mgmt
