#ifndef FILE_H_
#define FILE_H_

#include "pathname/Management.h"

#include <fstream>
#include <mutex>

class File
{
public:
    typedef std::streamoff OffsetType;
    typedef std::streampos PosisitionType;
    typedef std::streamsize SizeType;
    typedef pathname::Management::KeyValueType PathnameKeyType;
protected:
    std::mutex m_lock;
    std::filebuf m_filebuf;
private:
    std::ios_base::openmode m_openmode;
protected:
    File(std::ios_base::openmode);
public:
    File(const File &) = delete;
    File(File &&) = delete;
public:
    ~File();
public:
    File & operator=(const File &) = delete;
    File & operator=(File &&) = delete; 
public:
    bool IsOpen();
protected:
    bool Open(const Pathname & pathname);
public:
    bool Open(const PathnameKeyType & pathname_key);
public:
    void Close();
public:
    bool IsLock();
};

#endif //!FILE_H_