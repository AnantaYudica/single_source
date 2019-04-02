#ifndef INTF_RECORD_H_
#define INTF_RECORD_H_

#include "file/Input.h"
#include "file/Output.h"

#include "../defn/file/Offset.h"
#include "../defn/file/Position.h"
#include "../defn/file/Size.h"
#include "../defn/file/Way.h"
#include "../defn/rec/Status.h"

namespace intf
{

class Record
{
public:
    typedef defn::file::OffsetType OffsetType;
    typedef defn::file::PositionType PositionType;
    typedef defn::file::SizeType SizeType;
    typedef defn::file::Way WayType;
public:
    typedef file::Input InputType;
    typedef file::Output OutputType;
public:
    typedef defn::rec::StatusValueType StatusValueType;
    typedef defn::rec::Status StatusType;
protected:
    template<typename TReturnValue = int>
    static TReturnValue Bad(Record & rec, const TReturnValue & val = 0);
    template<typename TReturnValue = int>
    static TReturnValue Bad(const Record & rec, const TReturnValue & val = 0);
protected:
    template<typename TReturnValue = int>
    static TReturnValue Good(Record & rec, const TReturnValue & val = 0);
    template<typename TReturnValue = int>
    static TReturnValue Good(const Record & rec, const TReturnValue & val = 0);
protected:
    template<typename TReturnValue = int>
    static TReturnValue OutOfSynchronization(Record & rec, 
        const TReturnValue & val = 0);
    template<typename TReturnValue = int>
    static TReturnValue OutOfSynchronization(const Record & rec, 
        const TReturnValue & val = 0);
private:
    StatusValueType m_status;
public:
    Record();
public:
    Record(const Record & cpy);
    Record(Record && mov);
public:
    virtual ~Record() = default;
public:
    Record & operator=(const Record & cpy);
    Record & operator=(Record && mov);
public:
    virtual SizeType Put(OutputType & out) const = 0;
public:
    virtual SizeType Get(InputType & in) = 0;
public:
    bool IsInitial() const;
    bool IsBad() const;
    bool IsGood() const;
    bool IsOutOfSynchronization() const;
public:
    StatusValueType Status() const;
public:
    virtual bool operator==(const Record & rec) const = 0;
    virtual bool operator!=(const Record & rec) const = 0;
};

} //!intf

#endif //!INTF_RECORD_H_
