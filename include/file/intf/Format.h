#ifndef FILE_INTF_FORMAT_H_
#define FILE_INTF_FORMAT_H_

#include "../../intf/File.h"

namespace file
{
namespace intf
{

template<typename TKey>
class Format
{
public:
    typedef ::intf::File FileInterfaceType;
public:
    typedef typename FileInterfaceType::PositionType PositionType;
    typedef typename FileInterfaceType::OffsetType OffsetType;
    typedef typename FileInterfaceType::SizeType SizeType;
public:
    Format() = default;
    virtual ~Format() = default;
public:
    Format(const Format &) = default;
    Format(Format &&) = default;
public:
    virtual Format & operator=(const Format &) = 0;
    virtual Format & operator=(Format &&) = 0;
public:
    virtual PositionType Read(FileInterfaceType & file, const TKey & key, 
        char * data, SizeType size) = 0;
    virtual PositionType Write(FileInterfaceType & file, const TKey & key, 
        const char * data, SizeType size) = 0;
};

} //!intf

} //!file

#endif //!FILE_INTF_FORMAT_H_
