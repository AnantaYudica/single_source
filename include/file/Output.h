#ifndef FILE_OUTPUT_H_
#define FILE_OUTPUT_H_

#include "../File.h"
#include "../Pathname.h"

namespace file
{

class Output : public virtual File 
{
public:
    typedef typename File::OffsetType OffsetType;
    typedef typename File::PosisitionType PosisitionType;
    typedef typename File::SizeType SizeType;
public:
    Output();
public:
    Output(const Output & cpy) = delete;
    Output(Output && mov) = delete;
public:
    Output & operator=(const Output &) = delete;
    Output & operator=(Output &&) = delete;
public:
    SizeType Write(char * buffer, const SizeType & size);
public:
    PosisitionType SetPosition(const PosisitionType & pos);
    PosisitionType MovePosition(const OffsetType & off);
    PosisitionType BeginPosition();
    PosisitionType EndPosition();
    PosisitionType CurrentPosition();
};

}

#endif //!FILE_OUTPUT_H_