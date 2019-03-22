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

typename Filebuf::PositionType 
Filebuf::SeekPosition(const PositionType & pos, const ModeValueType & mode)
{
    const bool is_input_mode = (mode & (ModeValueType)ModeType::input); 
    const bool is_output_mode = (mode & (ModeValueType)ModeType::output); 
    int seek_mode = (is_input_mode ? ios_base::in : 0);
    seek_mode |= (is_output_mode ? ios_base::out : 0);
    if (seek_mode == 0)
        return m_filebuf.pubseekpos(pos);
    return m_filebuf.pubseekoff(pos, seek_mode);
}

typename Filebuf::PositionType 
Filebuf::SeekOffset(const OffsetType & off, const WayType & way, 
    const ModeValueType & mode)
{
    const bool is_input_mode = (mode & (ModeValueType)ModeType::input); 
    const bool is_output_mode = (mode & (ModeValueType)ModeType::output); 
    int seek_mode = (is_input_mode ? ios_base::in : 0);
    seek_mode |= (is_output_mode ? ios_base::out : 0);
    const int way_mode = (way == WayType::begin ? ios_base::beg :
        (way == WayType::current ? ios_base::cur : 
        (way == WayType::end ? ios_base::end : 0)));
    if (way_mode == 0) return -1;
    if (seek_mode == 0)
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

int Filebuf::Current()
{
    if (!m_filebuf.is_open()) return EOF;
    return m_filebuf.sgetc();
}

bool Filebuf::IsEndOfFile()
{
    return Current() == EOF;
}
