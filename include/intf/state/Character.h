#ifndef INTF_STATE_CHARACTER_H_
#define INTF_STATE_CHARACTER_H_

namespace intf
{
namespace state
{

class Character
{
protected:
    Character() = default;
public:
    virtual ~Character();
public:
    Character(const Character &) = default;
    Character(Character &&) = default;
public:
    Character & operator=(const Character &) = default;
    Character & operator=(Character &&) = default;
public:
    virtual void Next() = 0;
public:
    virtual char Get() const = 0;
public:
    virtual operator bool() const = 0;
public:
    virtual bool operator==(const Character & other) = 0;
    virtual bool operator!=(const Character & other) = 0;
    virtual bool operator<(const Character & other) = 0;
    virtual bool operator>(const Character & other) = 0;
};

} //!state

} //!intf

#endif //!INTF_STATE_CHARACTER_H_
