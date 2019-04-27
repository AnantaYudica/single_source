#include "mgmt/File.h"
#include "mgmt/Pathname.h"

namespace mgmt
{

File & File::GetInstance()
{
    static File instance;
    return instance;
}

File::File()
{}

File::~File()
{
    for(auto it = m_elements.begin(); it != m_elements.end(); it++)
    {
        delete it->second;
        delete m_mutexes[it->first];
    }
    m_elements.clear();
    m_counts.clear();
    m_mutexes.clear();
}

void File::Deallocate(FilePointerType & pointer)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto key = pointer.KeyPathname(); 
    auto fn = m_elements.find(key);
    if (fn != m_elements.end())
    {
        if (m_counts[key] == 0)
        {
            {
                std::lock_guard<std::mutex> element_lock(*(m_mutexes[key]));
                delete fn->second;
                m_elements.erase(key);
            }
            m_counts.erase(key);
            delete m_mutexes[key];
            m_mutexes.erase(key);
        }
    }
}

bool File::Open(const KeyPathnameType & key)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto fn = m_elements.find(key);
    if (fn == m_elements.end()) return false;
    std::lock_guard<std::mutex> element_lock(*(m_mutexes[key]));
    return fn->second->Open(mgmt::Pathname::GetInstance().
        Get(key).String()) != NULL;
}

void File::Close(const KeyPathnameType & key)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto fn = m_elements.find(key);
    if (fn == m_elements.end()) return;
    std::lock_guard<std::mutex> element_lock(*(m_mutexes[key]));
    fn->second->Close();
}

typename File::FilePointerType File::Get(const KeyPathnameType & key)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto fn = m_elements.find(key);
    if (fn == m_elements.end()) return FilePointerType();
    return FilePointerType(key, fn->second, m_mutexes[key], &m_counts[key]);
}

bool File::Has(const KeyPathnameType & key)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_elements.find(key) != m_elements.end();
}

void File::Lock(const KeyPathnameType & key)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto fn = m_mutexes.find(key);
    if (fn == m_mutexes.end()) return;
    fn->second->lock();
}

bool File::Trylock(const KeyPathnameType & key)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto fn = m_mutexes.find(key);
    if (fn == m_mutexes.end()) return false;
    return fn->second->try_lock();
}

void File::Unlock(const KeyPathnameType & key)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto fn = m_mutexes.find(key);
    if (fn == m_mutexes.end()) return;
    fn->second->unlock();
}

} //mgmt

