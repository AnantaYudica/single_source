#ifndef STATE_H_
#define STATE_H_

#include "intf/State.h"

#include <stack>

class State
{
public:
    State();
public:
    bool Symbol(const char & ch);
};

#endif //!STATE_H_
