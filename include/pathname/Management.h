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
    bool NextPathname(std::streampos & curr_pos);
    bool NextKey(std::streampos & curr_pos);
private:
    bool Find(const Pathname & pathname);
    bool Find(const KeyValueType & key); 
private:
    void Update(const KeyValueType & key);
    void Update(const KeyValueType & key, const Pathname & pathname);
public:
    KeyValueType GetKey(Pathname pathname);
    Pathname GetPathname(KeyValueType key);
public:
    bool Has(Pathname pathname);
    bool Has(KeyValueType key);
};

}

#endif //!PATHNAME_MANAGE_H_