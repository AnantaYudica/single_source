#ifndef FILE_INPUT_H_
#define FILE_INPUT_H_

#include "../File.h"
#include "../Pathname.h"

namespace file
{

class Input : public virtual File
{
public:
    typedef typename File::OffsetType OffsetType;
    typedef typename File::PosisitionType PosisitionType;
    typedef typename File::SizeType SizeType;
public:
    Input();
public:
    Input(const Input & cpy) = delete;
    Input(Input && mov) = delete;
public:
    Input & operator=(const Input &) = delete;
    Input & operator=(Input &&) = delete;
public:
    SizeType Read(char * buffer, const SizeType & size);
public:
    PosisitionType SetPosition(const PosisitionType & pos);
    PosisitionType MovePosition(const OffsetType & off);
    PosisitionType BeginPosition();
    PosisitionType EndPosition();
    PosisitionType CurrentPosition();
public:
    bool IsEndOfFile();
};

}

#endif //!FILE_INPUT_H_