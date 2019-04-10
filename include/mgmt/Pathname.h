#ifndef MGMT_PATHNAME_H_
#define MGMT_PATHNAME_H_

#include "../defn/mgmt/pathname/Key.h"
#include "../Pathname.h"
#include "pathname/Cache.h"
#include "pathname/Temporary.h"

#include <mutex>

namespace mgmt
{

class Pathname
{
public:
    typedef defn::mgmt::pathname::KeyValue KeyValueType;
    typedef ::Pathname PathnameType;
private:
    typedef pathname::Cache CacheType;
    typedef pathname::Temporary TemporaryType;
private:
    static Pathname ms_instance;
public:
    static Pathname & GetInstance();
private:
    CacheType m_cache;
    TemporaryType m_temporary;
    std::mutex m_mutex;
private:
    Pathname();
public:
    Pathname(const Pathname & cpy) = delete;
    Pathname(Pathname && mov) = delete;
public:
    Pathname & operator=(const Pathname & cpy) = delete;
    Pathname & operator=(Pathname && mov) = delete;
public:
    ~Pathname();
public:
    KeyValueType Register(const PathnameType & pathname);
public:
    PathnameType Unregister(const KeyValueType & key);
    void Unregister(const PathnameType & pathname);
public:
    PathnameType Get(const KeyValueType & key);
    KeyValueType Get(const PathnameType & pathname);
public:
    bool Has(const KeyValueType & key);
    bool Has(const PathnameType & pathname);
};

} //!mgmt

#endif //!MGMT_PATHNAME_H_
