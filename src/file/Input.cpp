#include "file/Input.h"

using namespace std;
using namespace file;

Input::Input() :
    File(ios_base::in)
{}


typename Input::SizeType 
Input::Read(char * buffer, const SizeType & size)
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return 0;
    return m_filebuf.sgetn(buffer, size);
}

typename Input::PosisitionType 
Input::SetPosition(const PosisitionType & pos)
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return -1;
    return m_filebuf.pubseekpos(pos, ios_base::in);
}

typename Input::PosisitionType 
Input::MovePosition(const OffsetType & off)
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return -1;
    return m_filebuf.pubseekoff(off, ios_base::cur, ios_base::in);
}

typename Input::PosisitionType 
Input::BeginPosition()
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return -1;
    return m_filebuf.pubseekpos(0, ios_base::in);

}

typename Input::PosisitionType 
Input::EndPosition()
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return -1;
    return m_filebuf.pubseekoff(0, ios_base::end, ios_base::in);
}

typename Input::PosisitionType 
Input::CurrentPosition()
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return -1;
    return m_filebuf.pubseekoff(0, ios_base::cur, ios_base::in);
}

bool Input::IsEndOfFile()
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return false;
    return m_filebuf.sgetc() == EOF;
}
