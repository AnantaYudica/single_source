#include "file/Output.h"

using namespace std;
using namespace file;

Output::Output() :
    File(ios_base::out)
{}

typename Output::SizeType 
Output::Write(char * buffer, const SizeType & size)
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return 0;
    return m_filebuf.sputn(buffer, size);
}

typename Output::PosisitionType 
Output::SetPosition(const PosisitionType & pos)
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return -1;
    return m_filebuf.pubseekpos(pos, ios_base::out);
}

typename Output::PosisitionType 
Output::MovePosition(const OffsetType & off)
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return -1;
    return m_filebuf.pubseekoff(off, ios_base::cur, ios_base::out);
}

typename Output::PosisitionType 
Output::BeginPosition()
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return -1;
    return m_filebuf.pubseekpos(0, ios_base::out);
}

typename Output::PosisitionType 
Output::EndPosition()
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return -1;
    return m_filebuf.pubseekoff(0, ios_base::end, ios_base::out);
}

typename Output::PosisitionType 
Output::CurrentPosition()
{
    lock_guard<mutex> lock{m_lock};
    if(!m_filebuf.is_open()) return -1;
    return m_filebuf.pubseekoff(0, ios_base::cur, ios_base::out);
}