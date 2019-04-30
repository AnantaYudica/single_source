#ifndef INTF_STATE_H_
#define INTF_STATE_H_

#include "state/Character.h"
#include "Message.decl.h"

namespace intf
{

class State
{
protected:
    State();
public:
    virtual ~State() = default;
public:
    State(const State &) = delete;
    State(State &&) = default;
public:
    State & operator=(const State &) = delete;
    State & operator=(State &&) = delete;
public:
    virtual bool Input(state::Character & ch) = 0;
public:
    virtual state::Message & Transition() = 0;
public:
    virtual bool operator==(const State & other) = 0;
    virtual bool operator!=(const State & other) = 0;
};

} //!intf

#endif //!INTF_STATE_H_
