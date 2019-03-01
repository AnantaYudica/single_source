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
private:
    std::mutex m_lock;
protected:
    std::filebuf m_filebuf;
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
    void Lock();
    void UnLock();
    bool TryLock();
};

#endif //!FILE_H_