#ifndef INTF_STATE_H_
#define INTF_STATE_H_

#include "state/Message.h"

namespace intf
{

class State
{
protected:
    State();
public:
    virtual ~State() = default;
public:
    virtual bool Symbol(const char & ch) = 0;
public:
    virtual state::Message Transition() = 0;
};

} //!intf

#endif //!INTF_STATE_H_
