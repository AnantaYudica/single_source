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
    m_count(0),
    m_cache(20),
    m_temporary()
{}

Pathname::~Pathname()
{}

typename Pathname::KeyValueType 
Pathname::Register(const PathnameType & pathname)
{
    lock_guard<mutex> lock(m_mutex);
    KeyValueType key = m_count++;
    m_cache.Insert(key, pathname);
    m_temporary.Insert(key, pathname);
}

typename Pathname::PathnameType 
Pathname::Unregister(const KeyValueType & key)
{
    m_cache.Remove(key);
    m_temporary.Remove(key);
}

void Pathname::Unregister(const PathnameType & pathname)
{
    m_cache.Remove(m_temporary.Get(pathname));
    m_temporary.Remove(pathname);
}

typename Pathname::PathnameType Pathname::Get(const KeyValueType & key)
{
    if (m_cache.Has(key))
        return m_cache.Get(key);
    return m_temporary.Get(key);
}

typename Pathname::KeyValueType Pathname::Get(const PathnameType & pathname)
{
    return m_temporary.Get(pathname);
}

bool Pathname::Has(const KeyValueType & key)
{
    return m_cache.Has(key) ||
        m_temporary.Has(key);
}

bool Pathname::Has(const PathnameType & pathname)
{
    return m_temporary.Get(pathname);
}

} //!mgmt
