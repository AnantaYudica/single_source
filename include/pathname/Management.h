#ifndef PATHNAME_MANAGE_H_
#define PATHNAME_MANAGE_H_

#include "../Pathname.h"

#include <map>
#include <mutex>
#include <fstream>
#include <string>
#include <deque>

namespace pathname
{

class Management
{
public:
    typedef int KeyValueType;
private:
    static Management ms_instance;
    static constexpr std::size_t ms_pathname_alloc_size = 256;
    static constexpr std::size_t ms_key_alloc_size = 21;
    static constexpr std::size_t ms_pathname_offset = 0;
    static constexpr std::size_t ms_key_offset = 257;
    static constexpr std::size_t ms_end_offset = 279;
    static constexpr char ms_empty_ch = ' ';
    static constexpr char ms_conj_ch = '$';
    static constexpr char ms_end_ch = '\n';
public:
    static Management & GetInstance();
private:
    std::size_t m_size;
    std::map<KeyValueType, Pathname> m_pathname_map;
    std::deque<KeyValueType> m_key_queue;
    std::string m_pathname_str;
    std::filebuf m_buff;
    std::mutex m_lock;
private:
    Management();
public:
    Management(const Management &) = delete;
    Management(Management &&) = delete;
public:
    ~Management();
public:
    Management & operator=(const Management &) = delete;
    Management & operator=(Management &&) = delete;
private:
    bool Open(); 
private:
    void Push(const Pathname & pathname, const KeyValueType & key);
private:
    Pathname GetPathname();
    KeyValueType GetKey();
private:
    void Back(std::streampos & curr_pos);
    void Back(const std::ios_base::openmode & mode);
private:
    bool NextPathname(const std::ios_base::openmode & mode);
    bool NextKey(const std::ios_base::openmode & mode);
private:
    bool Find(const Pathname & pathname, 
        const std::ios_base::openmode & mode);
    bool Find(const KeyValueType & key, 
        const std::ios_base::openmode & mode);
    bool Find(const KeyValueType & key, Pathname & pathname,
        const std::ios_base::openmode & mode); 
private:
    void Update(const KeyValueType & key);
    void Update(const KeyValueType & key, const Pathname & pathname);
private:
    void Erase();
public:
    KeyValueType Register(Pathname pathname);
public:
    Pathname Unregister(KeyValueType key);
    void Unregister(Pathname pathname);
public:
    Pathname Get(KeyValueType key);
    KeyValueType Get(Pathname pathname);
public:
    bool Has(KeyValueType key);
    bool Has(Pathname pathname);
};

}

#endif //!PATHNAME_MANAGE_H_