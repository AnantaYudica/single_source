#ifndef MGMT_FILE_H_
#define MGMT_FILE_H_

#include "../intf/File.h"
#include "../defn/mgmt/pathname/Key.h"
#include "file/Pointer.h"

#include <map>
#include <memory>
#include <mutex>
#include <cstddef>
#include <utility>

namespace mgmt
{

class File
{
public:
    typedef defn::mgmt::pathname::KeyValue KeyPathnameType;
    typedef intf::File FileInterfaceType;
    typedef file::Pointer FilePointerType;
public:
    static File & GetInstance();
private:
    std::map<KeyPathnameType, FileInterfaceType *> m_elements;
    std::map<KeyPathnameType, std::size_t> m_counts;
    std::map<KeyPathnameType, std::mutex *> m_mutexes;
    std::mutex m_mutex;
private:
    File();
public:
    ~File();
public:
    template<typename TImplementation, typename ... TArgs>
    FilePointerType Allocate(const KeyPathnameType & key, TArgs && ... args);
public:
    void Deallocate(FilePointerType & pointer);
public:
    bool Open(const KeyPathnameType & key);
public:
    void Close(const KeyPathnameType & key);
public:
    void Bind(const KeyPathnameType & key);
public:
    FilePointerType Get(const KeyPathnameType & key);
public:
    bool Has(const KeyPathnameType & key);
public:
    void Lock(const KeyPathnameType & key);
public:
    bool Trylock(const KeyPathnameType & key);
public:
    void Unlock(const KeyPathnameType & key);
};

template<typename TImplementation, typename ... TArgs>
typename File::FilePointerType File::Allocate(const KeyPathnameType & key,
    TArgs && ... args)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto fn = m_elements.find(key);
    if (fn == m_elements.end())
    {
        FileInterfaceType * new_element = 
            new TImplementation(std::forward<TArgs>(args)...);
        m_elements.insert(std::make_pair(key, new_element));
        m_counts.insert(std::make_pair(key, 1));
        m_mutexes.insert(std::make_pair(key, new std::mutex()));
        return FilePointerType(key, new_element, m_mutexes[key]);
    }
    return FilePointerType(key, fn->second, m_mutexes[key]);
}

} //!mgmt

#endif //!MGMT_FILE_H_
