#ifndef INTF_STATE_MESSAGE_H_
#define INTF_STATE_MESSAGE_H_

#include "Message.decl.h"
#include "../../defn/state/msg/Tag.h"
#include "Character.h"
#include "State.h"

namespace intf
{
namespace state
{

class Message
{
public:
    typedef defn::state::msg::Tag TagType;
private:
    TagType m_tag;
protected:
    Message(const TagType & tag);
public:
    virtual ~Message() = default;
public:
    Message(const Message & cpy);
    Message(Message && mov);
public:
    Message & operator=(const Message & cpy);
    Message & operator=(Message && mov);
public:
    TagType Tag() const;
public:
    virtual Character & Begin() = 0;
    virtual Character & End() = 0;
public:
    virtual Character & RawBegin() = 0;
    virtual Character & RawEnd() = 0;
public:
    virtual State & From() = 0;
    virtual State & To() = 0;
};

} //!state

} //!intf


#endif //!INTF_STATE_MESSAGE_H_
