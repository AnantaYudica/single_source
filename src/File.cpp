#include "File.h"

using namespace std;

File::File(ios_base::openmode mode) :
    m_openmode{mode}
{}

File::~File()
{
    Close();
}

bool File::IsOpen()
{
    lock_guard<mutex> lock(m_lock);
    return m_filebuf.is_open();
}

bool File::Open(const Pathname & pathname)
{
    if (!pathname) return false;
    if (m_filebuf.is_open()) m_filebuf.close();
    m_filebuf.open(pathname.String(), m_openmode);
}

bool File::Open(const PathnameKeyType & pathname_key)
{
    if (pathname_key < 0) return false;
    lock_guard<mutex> lock(m_lock);
    auto pathname = pathname::Management::GetInstance().
        Get(pathname_key);
    return Open(pathname);
}
void File::Close()
{
    lock_guard<mutex> lock(m_lock);
    if (m_filebuf.is_open()) m_filebuf.close();
}

bool File::IsLock()
{
    if(m_lock.try_lock())
    {
        m_lock.unlock();
        return false;
    }
    return true;
}
