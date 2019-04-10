#ifndef FILE_FORMAT_LINEAR
#define FILE_FORMAT_LINEAR

#include "../../intf/File.h"
#include "../../intf/file/Input.h"
#include "../../intf/file/Output.h"
#include "../../intf/Record.h"
#include "../../defn/file/Mode.h"

#include <memory>

namespace file
{
namespace format
{

class Linear
{
public:
    typedef std::shared_ptr<Linear> PointerType;
public:
    typedef intf::File FileInterfaceType;
    typedef typename FileInterfaceType::PointerType
        FileInterfacePointerType;
public:
    typedef intf::Record RecordInterfaceType;
public:
    typedef typename FileInterfaceType::PositionType PositionType;
    typedef typename FileInterfaceType::OffsetType OffsetType;
    typedef typename FileInterfaceType::SizeType SizeType;
    typedef typename FileInterfaceType::ModeType ModeType;
    typedef typename FileInterfaceType::ModeValueType ModeValueType;
    typedef typename FileInterfaceType::WayType WayType;
public:
    typedef intf::file::Input FileInputType;
    typedef intf::file::Output FileOutputType;
private:
    SizeType m_elem_size;
    FileInterfacePointerType m_file;
public:
    Linear();
    Linear(FileInterfacePointerType file, const SizeType & elem_size);
public:
    ~Linear();
public:
    Linear(const Linear & cpy);
    Linear(Linear && mov);
public:
    Linear & operator=(const Linear & cpy);
    Linear & operator=(Linear && mov);
public:
    SizeType ElementSize() const;
    FileInterfacePointerType File();
public:
    PositionType SeekPosition(const PositionType & pos, 
        const ModeValueType & mode = defn::file::mode::DefaultValue);
    PositionType SeekOffset(const OffsetType & off, const WayType & way,
        const ModeValueType & mode = defn::file::mode::DefaultValue);
public:
    SizeType Put(const char * buffer, const SizeType & size);
    SizeType Put(const RecordInterfaceType & rec);
public:
    SizeType Get(char * buffer, const SizeType & size);
    SizeType Get(RecordInterfaceType & rec);
public:
    SizeType CurrentPut(const char * buffer, const SizeType & size);
    SizeType CurrentPut(const OffsetType & offset, const char * buffer, 
        const SizeType & size);
    SizeType CurrentPut(const RecordInterfaceType & rec);
    SizeType CurrentPut(const OffsetType & offset, 
        const RecordInterfaceType & rec);
public:
    SizeType CurrentGet(char * buffer, const SizeType & size);
    SizeType CurrentGet(const OffsetType & offset, char * buffer, 
        const SizeType & size);
    SizeType CurrentGet(RecordInterfaceType & rec);
    SizeType CurrentGet(const OffsetType & offset, RecordInterfaceType & rec);
public:
    SizeType Size();
};

} //!format

} //!file

#endif //!FILE_FORMAT_LINEAR
