#ifndef FILE_INPUT_H_
#define FILE_INPUT_H_

#include <fstream>
#include <mutex>

#include "Pathname.h"

namespace file
{

class Input
{
private:
    typename Pathname::WeakPointerType m_in_pathname_ptr;
    std::filebuf m_in_filebuf;
    std::mutex m_lock;
public:
    Input();
    Input(typename Pathname::WeakPointerType in_pathname_ptr);
public:
    Input(const Input & cpy) = delete;
    Input(Input && mov) = delete;
public:
    Input & operator=(const Input &) = delete;
    Input & operator=(Input &&) = delete;
public:
    ~Input();
public:
    bool Open(typename Pathname::WeakPointerType in_pathname_ptr);
public:
    std::size_t Read(char * buffer, const std::size_t & size);
public:
    bool IsEndOfFile();
    bool IsOpen();
public:
    void Lock();
    void UnLock();
    bool TryLock();
};

}

#endif //!FILE_INPUT_H_