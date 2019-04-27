#include "mgmt/file/Pointer.h"
#include "mgmt/File.h"

namespace mgmt
{
namespace file
{

Pointer::Pointer() :
    m_lock(false),
    m_mutex(nullptr),
    m_key(-1),
    m_file(nullptr),
    m_count(nullptr)
{}

Pointer::Pointer(const KeyPathnameType & key, intf::File * file, 
    std::mutex * lock, std::atomic_size_t * count) :
        m_lock(false),
        m_mutex(lock),
        m_key(key),
        m_file(file),
        m_count(count)
{
    if (m_key != -1) ++(*count);
}

Pointer::~Pointer()
{
    Reset();
}

Pointer::Pointer(const Pointer & cpy) :
    m_lock(false),
    m_mutex(cpy.m_mutex),
    m_key(cpy.m_key),
    m_file(cpy.m_file),
    m_count(cpy.m_count)
{
    if (m_key != -1) ++(*m_count);
}

Pointer::Pointer(Pointer && mov) :
    m_lock(mov.m_lock),
    m_mutex(mov.m_mutex),
    m_key(mov.m_key),
    m_file(mov.m_file),
    m_count(mov.m_count)
{
    mov.m_mutex = nullptr;
    mov.m_key = -1;
    mov.m_file = nullptr;
    mov.m_count = nullptr;
}

Pointer & Pointer::operator=(const Pointer & cpy)
{
    Reset();
    m_lock = false;
    m_mutex = cpy.m_mutex;
    m_key = cpy.m_key;
    m_file = cpy.m_file;
    m_count = cpy.m_count;
    if (m_key != -1) ++(*m_count);
    return *this;
}

Pointer & Pointer::operator=(Pointer && mov)
{
    Reset();
    m_lock = mov.m_lock;
    m_mutex = mov.m_mutex;
    m_key = mov.m_key;
    m_file = mov.m_file;
    m_count = mov.m_count;
    return *this;
}

void Pointer::Reset()
{
    if (m_key != -1)
    {
        if (m_lock) m_mutex->unlock();
        --m_count;
        mgmt::File::GetInstance().Deallocate(*this);
    }
    m_lock = false;
    m_mutex = nullptr;
    m_key = -1;
    m_file = nullptr;
    m_count = nullptr;
}

typename Pointer::KeyPathnameType Pointer::KeyPathname() const
{
    return m_key;
}

bool Pointer::IsOpen() const
{
    if (m_key == -1) return false;
    if (m_lock) return m_file->IsOpen();
    std::lock_guard<std::mutex> lock(*m_mutex);
    return m_file->IsOpen();
}

typename Pointer::PositionType 
Pointer::SeekPosition(const PositionType & pos, const ModeValueType & mode)
{
    if (m_key == -1) return 0;
    if (m_lock) return m_file->SeekPosition(pos, mode);
    std::lock_guard<std::mutex> lock(*m_mutex);
    return m_file->SeekPosition(pos, mode);
}

typename Pointer::PositionType 
Pointer::SeekOffset(const OffsetType & off, const WayType & way, 
    const ModeValueType & mode)
{
    if (m_key == -1) return 0;
    if (m_lock) return m_file->SeekOffset(off, way, mode);
    std::lock_guard<std::mutex> lock(*m_mutex);
    return m_file->SeekOffset(off, way, mode);
}

typename Pointer::SizeType 
Pointer::Put(const char * buffer, const SizeType & size)
{
    if (m_key == -1) return 0;
    if (m_lock) return m_file->Put(buffer, size);
    std::lock_guard<std::mutex> lock(*m_mutex);
    return m_file->Put(buffer, size);
}

typename Pointer::SizeType Pointer::Put(const char & ch)
{
    if (m_key == -1) return 0;
    if (m_lock) return m_file->Put(ch);
    std::lock_guard<std::mutex> lock(*m_mutex);
    return m_file->Put(ch);
}

typename Pointer::SizeType 
Pointer::CurrentPut(const char * buffer, const SizeType & size)
{
    if (m_key == -1) return 0;
    if (m_lock) return m_file->CurrentPut(buffer, size);
    std::lock_guard<std::mutex> lock(*m_mutex);
    return m_file->CurrentPut(buffer, size);
}

typename Pointer::SizeType Pointer::CurrentPut(const char & ch)
{
    if (m_key == -1) return 0;
    if (m_lock) return m_file->CurrentPut(ch);
    std::lock_guard<std::mutex> lock(*m_mutex);
    return m_file->CurrentPut(ch);
}

typename Pointer::SizeType Pointer::Get(char * buffer, const SizeType & size)
{
    if (m_key == -1) return 0;
    if (m_lock) return m_file->Get(buffer, size);
    std::lock_guard<std::mutex> lock(*m_mutex);
    return m_file->Get(buffer, size);
}

int Pointer::Get()
{
    if (m_key == -1) return EOF;
    if (m_lock) return m_file->Get();
    std::lock_guard<std::mutex> lock(*m_mutex);
    return m_file->Get();
}

typename Pointer::SizeType 
Pointer::CurrentGet(char * buffer, const SizeType & size)
{
    if (m_key == -1) return 0;
    if (m_lock) return m_file->CurrentGet(buffer, size);
    std::lock_guard<std::mutex> lock(*m_mutex);
    return m_file->CurrentGet(buffer, size);
}

int Pointer::CurrentGet()
{
    if (m_key == -1) return EOF;
    if (m_lock) return m_file->CurrentGet();
    std::lock_guard<std::mutex> lock(*m_mutex);
    return m_file->CurrentGet();
}

void Pointer::Lock()
{
    if (m_key == -1) return;
    if (!m_lock) 
    {
        m_mutex->lock();
        m_lock = true;
    }
}

bool Pointer::Trylock()
{
    if (m_key == -1) return false;
    if (!m_lock && m_mutex->try_lock()) m_lock = true;
    return m_lock;
}

void Pointer::Unlock()
{
    if (m_key == -1) return;
    if (m_lock)
    {
        m_mutex->unlock();
        m_lock = false;
    }
}

Pointer::operator bool() const
{
    return m_key != -1 && m_file;
}

bool Pointer::operator==(const Pointer & other) const
{
    return m_key == other.m_key && m_file == other.m_file;
}

bool Pointer::operator!=(const Pointer & other) const
{
    return !(*this == other);
}

} //!file

} //!mgmt
