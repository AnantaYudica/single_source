#ifndef FILE_IMP_STD_FILEBUF_H_
#define FILE_IMP_STD_FILEBUF_H_

#include "../../../intf/File.h"

#include <string>
#include <fstream>

namespace file
{
namespace imp
{
namespace std
{

class Filebuf :
    public intf::File
{
public:
    typedef intf::File FileInterfaceType;
public:
    typedef typename intf::File::PositionType PositionType;
    typedef typename intf::File::OffsetType OffsetType;
    typedef typename intf::File::SizeType SizeType;
public:
    typedef typename intf::File::ModeType ModeType;
    typedef typename intf::File::ModeValueType ModeValueType;
    typedef typename intf::File::WayType WayType;
private:
    ::std::filebuf m_filebuf;
    ::std::ios_base::openmode m_openMode;
public:
    Filebuf(const ::std::ios_base::openmode & open_mode);
public:
    ~Filebuf();
public:
    Filebuf(const Filebuf &) = delete;
    Filebuf(Filebuf && mov);
public:
    Filebuf & operator=(const Filebuf &) = delete;
    FileInterfaceType & operator=(FileInterfaceType && mov);
public:
    bool IsOpen();
public:
    FileInterfaceType * Open(const ::std::string & pathname);
    FileInterfaceType * Open(const char * pathname);
public:
    void Close();
public:
    ModeValueType Mode() const;
public:
    PositionType SeekPosition(const PositionType & pos,
        const ModeValueType & mode = intf::File::ms_default_mode); 
    PositionType SeekOffset(const OffsetType & off,
        const WayType & way, const ModeValueType & mode = 
            intf::File::ms_default_mode);
public:
    SizeType Put(const char * buffer, const SizeType & size);
    SizeType Put(const char & ch);
public:
    SizeType Get(char * buffer, const SizeType & size);
    int Get();
public:
    SizeType CurrentPut(const char * buffer, const SizeType & size);
    SizeType CurrentPut(const char & ch);
public:
    SizeType CurrentGet(char * buffer, const SizeType & size);
    int CurrentGet();
public:
    bool IsEndOfFile();
};

} //!std

} //!imp

} //!file

#endif //!FILE_IMP_STD_FILEBUF_H_