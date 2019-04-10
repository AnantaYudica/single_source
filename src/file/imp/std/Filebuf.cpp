#include "file/imp/std/Filebuf.h"

#include <utility>

using namespace std;
using namespace file::imp::std;

Filebuf::Filebuf(const ::std::ios_base::openmode & open_mode) :
    m_openMode(open_mode)
{}

Filebuf::~Filebuf()
{}

Filebuf::Filebuf(Filebuf && mov) :
    m_filebuf(move(mov.m_filebuf))
{}

typename Filebuf::FileInterfaceType & 
Filebuf::operator=(FileInterfaceType && mov)
{
    auto * file_buf = dynamic_cast<Filebuf *>(&mov);
    if (!file_buf) return *this;
    m_filebuf = move(file_buf->m_filebuf);
    return *this;
}

bool Filebuf::IsOpen()
{
    return m_filebuf.is_open();
}

typename Filebuf::FileInterfaceType * 
Filebuf::Open(const ::std::string & pathname)
{
    if (m_filebuf.open(pathname, m_openMode))
        return this;
    return nullptr;
}

typename Filebuf::FileInterfaceType * 
Filebuf::Open(const char * pathname)
{
    if (m_filebuf.open(pathname, m_openMode))
        return this;
    return nullptr;
}

void Filebuf::Close()
{
    m_filebuf.close();
}

typename Filebuf::ModeValueType Filebuf::Mode() const
{
    return (m_openMode & ios_base::in ? (ModeValueType)ModeType::input : 0) |
        (m_openMode & ios_base::out ? (ModeValueType)ModeType::output : 0);
}

typename Filebuf::PositionType 
Filebuf::SeekPosition(const PositionType & pos, const ModeValueType & mode)
{
    const bool is_input_mode = (mode & (ModeValueType)ModeType::input); 
    const bool is_output_mode = (mode & (ModeValueType)ModeType::output); 
    ios_base::openmode seek_mode = (is_input_mode ? ios_base::in : 
        ios_base::openmode());
    seek_mode |= (is_output_mode ? ios_base::out : ios_base::openmode());
    if (seek_mode == 0)
        return m_filebuf.pubseekpos(pos);
    return m_filebuf.pubseekpos(pos, seek_mode);
}

typename Filebuf::PositionType 
Filebuf::SeekOffset(const OffsetType & off, const WayType & way, 
    const ModeValueType & mode)
{
    const bool is_input_mode = (mode & (ModeValueType)ModeType::input); 
    const bool is_output_mode = (mode & (ModeValueType)ModeType::output); 
    ios_base::openmode seek_mode = (is_input_mode ? ios_base::in : 
        ios_base::openmode());
    seek_mode |= (is_output_mode ? ios_base::out :  ios_base::openmode());
    const ios_base::seekdir way_mode = (way == WayType::begin ? ios_base::beg :
        (way == WayType::current ? ios_base::cur : 
        (way == WayType::end ? ios_base::end :  ios_base::seekdir())));
    if (way_mode ==  ios_base::seekdir()) return -1;
    if (seek_mode == ios_base::openmode())
        return m_filebuf.pubseekoff(off, way_mode);
    return m_filebuf.pubseekoff(off, way_mode, seek_mode);
}

typename Filebuf::SizeType 
Filebuf::Put(const char * buffer, const SizeType & size)
{
    if (!buffer || size == 0 || !m_filebuf.is_open()) return 0;
    const auto size_ch = m_filebuf.sputn(buffer, size);
    return size_ch != EOF ? size_ch : 0;
}

typename Filebuf::SizeType Filebuf::Put(const char & ch)
{
    if (!m_filebuf.is_open()) return 0;
    return (m_filebuf.sputc(ch) != EOF ? 1 : 0);
}

typename Filebuf::SizeType Filebuf::Put(const char & ch, 
    const SizeType & count)
{
    if (!m_filebuf.is_open()) return 0;
    for (SizeType i = 0; i < count; ++i)
        if (m_filebuf.sputc(ch) == EOF) return i;
    return count;
}

typename Filebuf::SizeType Filebuf::Get(char * buffer, 
    const SizeType & size)
{
    if (!buffer || size == 0 || !m_filebuf.is_open()) return 0;
    const auto size_ch = m_filebuf.sgetn(buffer, size);
    return size_ch != EOF ? size_ch : 0;
}

int Filebuf::Get()
{
    if (!m_filebuf.is_open()) return EOF;
    return m_filebuf.sbumpc();
}

typename Filebuf::SizeType Filebuf::CurrentPut(const char * buffer, 
    const SizeType & size)
{
    if (!buffer || size == 0 || !m_filebuf.is_open()) return 0;
    auto size_ch = m_filebuf.sputn(buffer, size);
    if (size_ch == EOF) return 0;
    m_filebuf.pubseekoff(-size_ch, ios_base::cur, ios_base::out);
    return size_ch;
}

typename Filebuf::SizeType Filebuf::CurrentPut(const char & ch)
{
    if (!m_filebuf.is_open()) return 0;
    auto size_ch = m_filebuf.sputc(ch);
    if (size_ch == EOF) return 0;
    m_filebuf.pubseekoff(-1, ios_base::cur, ios_base::out);
    return 1;
}

typename Filebuf::SizeType Filebuf::CurrentPut(const char & ch, 
    const SizeType & count)
{
    if (!m_filebuf.is_open()) return 0;
    SizeType i = 0;
    while(i < count)
    {
        if (m_filebuf.sputc(ch) == EOF) break;
        ++i;
    }
    m_filebuf.pubseekoff(-i, ios_base::cur, ios_base::out);
    return i;
}

typename Filebuf::SizeType Filebuf::CurrentGet(char * buffer, 
    const SizeType & size)
{
    if (!buffer || size == 0 || !m_filebuf.is_open()) return 0;
    auto size_ch = m_filebuf.sgetn(buffer, size);
    if (size_ch == EOF) return 0;
    m_filebuf.pubseekoff(-size_ch, ios_base::cur, ios_base::in);
    return size_ch;
}

int Filebuf::CurrentGet()
{
    if (!m_filebuf.is_open()) return EOF;
    return m_filebuf.sgetc();
}

bool Filebuf::IsEndOfFile()
{
    return CurrentGet() == EOF;
}
