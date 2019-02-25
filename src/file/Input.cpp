#include "file/Input.h"

using namespace std;
using namespace file;

Input::Input()
{}

Input::Input(typename Pathname::WeakPointerType in_pathname_ptr) :
    m_in_pathname_ptr(in_pathname_ptr)
{
    auto lock_in_pathname_ptr = m_in_pathname_ptr.lock();
    m_in_filebuf.open(lock_in_pathname_ptr->GetPathname(), ios::in);
}

Input::~Input()
{
    if(m_in_filebuf.is_open())
        m_in_filebuf.close();
}

bool Input::Open(typename Pathname::WeakPointerType in_pathname_ptr)
{
    m_in_pathname_ptr = in_pathname_ptr;
    auto lock_in_pathname_ptr = m_in_pathname_ptr.lock();
    return m_in_filebuf.open(lock_in_pathname_ptr->GetPathname(), ios::in);
}

std::size_t Input::Read(char * buffer, const std::size_t & size)
{
    if(!m_in_filebuf.is_open()) return 0;
    return m_in_filebuf.sgetn(buffer, size);
}

bool Input::IsEndOfFile()
{
    if(!m_in_filebuf.is_open()) return false;
    return m_in_filebuf.sgetc() == EOF;
}

bool Input::IsOpen()
{
    return m_in_filebuf.is_open();
}

void Input::Lock()
{
    return m_lock.lock();
}

void Input::UnLock()
{
    return m_lock.unlock();
}

bool Input::TryLock()
{
    return m_lock.try_lock();
}
