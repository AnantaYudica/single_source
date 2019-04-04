#include "intf/Record.h"

#include <utility>

using namespace std;
using namespace intf;

Record::Record() :
    m_status((StatusValueType)StatusType::initial)
{}

Record::Record(const Record & cpy) :
    m_status(cpy.m_status)
{}

Record::Record(Record && mov) :
    m_status(move(mov.m_status))
{
    mov.m_status = (StatusValueType)StatusType::initial;
}

Record & Record::operator=(const Record & cpy)
{
    m_status = cpy.m_status;
    return *this;
}

Record & Record::operator=(Record && mov)
{
    m_status = move(mov.m_status);
    mov.m_status = (StatusValueType)StatusType::initial;
    return *this;
}

bool Record::IsInitial() const
{
    return m_status & (StatusValueType)StatusType::initial;
}

bool Record::IsBad() const
{
    return m_status & (StatusValueType)StatusType::bad;
}

bool Record::IsGood() const
{
    return m_status & (StatusValueType)StatusType::good;
}

bool Record::IsOutOfSynchronization() const
{
    return m_status & (StatusValueType)StatusType::out_of_sync;
}

typename Record::StatusValueType Record::Status() const
{
    return m_status;
}
