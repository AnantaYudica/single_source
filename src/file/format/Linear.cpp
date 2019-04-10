#include "file/format/Linear.h"

#include <utility>

using namespace std;
using namespace file::format;

Linear::Linear() :
    m_file(nullptr),
    m_elem_size(0)
{}

Linear::Linear(FileInterfacePointerType file, const SizeType & elem_size) :
    m_file(file),
    m_elem_size(elem_size)
{}

Linear::~Linear()
{
    m_file.reset();
    m_elem_size = 0;
}

Linear::Linear(const Linear & cpy) :
    m_file(cpy.m_file),
    m_elem_size(cpy.m_elem_size)
{}

Linear::Linear(Linear && mov) :
    m_file(move(mov.m_file)),
    m_elem_size(move(mov.m_elem_size))
{
    mov.m_file.reset();
    mov.m_elem_size = 0;
}

Linear & Linear::operator=(const Linear & cpy)
{
    m_file = cpy.m_file;
    m_elem_size = cpy.m_elem_size;
}

Linear & Linear::operator=(Linear && mov)
{
    m_file = move(mov.m_file);
    m_elem_size = move(mov.m_elem_size);
    mov.m_file.reset();
    mov.m_elem_size = 0;
}

typename Linear::SizeType Linear::ElementSize() const
{
    return m_elem_size;
}

typename Linear::FileInterfacePointerType Linear::File()
{
    return m_file;
}

typename Linear::PositionType Linear::SeekPosition(const PositionType & pos,
    const ModeValueType & mode)
{
    if (!m_file) return 0;
    return m_file->SeekPosition(pos * m_elem_size, mode);
}

typename Linear::PositionType Linear::SeekOffset(const OffsetType & off, 
    const WayType & way, const ModeValueType & mode)
{
    if (!m_file) return 0;
    return m_file->SeekOffset(off * m_elem_size, way, mode);
}

typename Linear::SizeType Linear::Put(const RecordInterfaceType & rec)
{
    if (!m_file) return 0;
    const auto pos = m_file->SeekOffset(0, WayType::current,
        (ModeValueType)ModeType::output);
    auto size_ch = rec.Put(*m_file);
    if (size_ch == EOF) return 0;
    m_file->SeekPosition(pos + m_elem_size, (ModeValueType)ModeType::output);
    return size_ch;
}  

typename Linear::SizeType Linear::Put(const char * buffer, 
    const SizeType & size)
{
    if (!m_file || !buffer || size == 0) return 0;
    SizeType max_size = size < m_elem_size ? size : m_elem_size;
    auto size_ch = m_file->Put(buffer, max_size);
    if (size_ch < m_elem_size)
        m_file->SeekOffset(m_elem_size - size_ch, WayType::current,
            (ModeValueType)ModeType::output);
    return size_ch;
}

typename Linear::SizeType Linear::Get(char * buffer, const SizeType & size)
{
    if (!m_file || !buffer || size == 0) return 0;
    SizeType max_size = size < m_elem_size ? size : m_elem_size;
    auto size_ch = m_file->Get(buffer, max_size);
    if (size_ch < m_elem_size)
        m_file->SeekOffset(m_elem_size - size_ch, WayType::current,
            (ModeValueType)ModeType::input);
    return size_ch;
}

typename Linear::SizeType Linear::Get(RecordInterfaceType & rec)
{
    if (!m_file) return 0;
    const auto pos = m_file->SeekOffset(0, WayType::current,
        (ModeValueType)ModeType::input);
    auto size_ch = rec.Get(*m_file);
    if (size_ch == EOF) return 0;
    m_file->SeekPosition(pos + m_elem_size, (ModeValueType)ModeType::input);
    return size_ch;
}

typename Linear::SizeType Linear::CurrentPut(const char * buffer, 
    const SizeType & size)
{
    if (!m_file || !buffer || size == 0) return 0;
    SizeType max_size = size < m_elem_size ? size : m_elem_size;
    auto size_ch = m_file->CurrentPut(buffer, max_size);
    return size_ch;
}

typename Linear::SizeType Linear::CurrentPut(const OffsetType & offset, 
    const char * buffer, const SizeType & size)
{
    if (!m_file || !buffer || size == 0 || offset < 0 ||
        offset >= m_elem_size) return 0;
    const SizeType elem_size = m_elem_size - offset;
    SizeType max_size = size < elem_size ? size : elem_size;
    auto size_ch = m_file->CurrentPut(buffer, max_size);
    return size_ch;
}

typename Linear::SizeType Linear::CurrentPut(const RecordInterfaceType & rec)
{
    if (!m_file) return 0;
    const auto pos = m_file->SeekOffset(0, WayType::current,
        (ModeValueType)ModeType::output);
    auto size_ch = rec.Put(*m_file);
    if (size_ch == EOF) return 0;
    m_file->SeekPosition(pos, (ModeValueType)ModeType::output);
    return size_ch;
}

typename Linear::SizeType Linear::CurrentPut(const OffsetType & offset, 
    const RecordInterfaceType & rec)
{
    if (!m_file || offset < 0 || offset >= m_elem_size) return 0;
    const auto pos = m_file->SeekOffset(0, WayType::current,
        (ModeValueType)ModeType::output);
    m_file->SeekOffset(offset, WayType::current,
        (ModeValueType)ModeType::output);
    auto size_ch = rec.Put(*m_file);
    if (size_ch == EOF) return 0;
    m_file->SeekPosition(pos, (ModeValueType)ModeType::output);
    return size_ch;
}

typename Linear::SizeType Linear::CurrentGet(char * buffer, 
    const SizeType & size)
{
    if (!m_file || !buffer || size == 0) return 0;
    SizeType max_size = size < m_elem_size ? size : m_elem_size;
    auto size_ch = m_file->CurrentGet(buffer, max_size);
    return size_ch;
}

typename Linear::SizeType Linear::CurrentGet(const OffsetType & offset, 
    char * buffer, const SizeType & size)
{
    if (!m_file || !buffer || size == 0 || offset < 0 ||
        offset >= m_elem_size) return 0;
    const SizeType elem_size = m_elem_size - offset;
    SizeType max_size = size < elem_size ? size : elem_size;
    auto size_ch = m_file->CurrentGet(buffer, max_size);
    return size_ch;
}

typename Linear::SizeType Linear::CurrentGet(RecordInterfaceType & rec)
{
    if (!m_file) return 0;
    const auto pos = m_file->SeekOffset(0, WayType::current,
        (ModeValueType)ModeType::input);
    auto size_ch = rec.Get(*m_file);
    if (size_ch == EOF) return 0;
    m_file->SeekPosition(pos, (ModeValueType)ModeType::input);
    return size_ch;
}

typename Linear::SizeType Linear::CurrentGet(const OffsetType & offset, 
    RecordInterfaceType & rec)
{
    if (!m_file || offset < 0 || offset >= m_elem_size) return 0;
    const auto pos = m_file->SeekOffset(0, WayType::current,
        (ModeValueType)ModeType::input);
    m_file->SeekOffset(offset, WayType::current,
        (ModeValueType)ModeType::input);
    auto size_ch = rec.Get(*m_file);
    if (size_ch == EOF) return 0;
    m_file->SeekPosition(pos, (ModeValueType)ModeType::input);
    return size_ch; 
}

typename Linear::SizeType Linear::Size()
{
    if (!m_file || !m_file->IsOpen()) return 0;
    auto pos = m_file->SeekOffset(0, WayType::end);
    if (pos == EOF) return 0;
    return SizeType(pos / m_elem_size);
}
