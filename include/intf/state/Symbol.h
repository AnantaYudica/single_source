#ifndef INTF_STATE_SYMBOL_H_
#define INTF_STATE_SYMBOL_H_

namespace intf
{
namespace state
{

class Symbol
{
protected:
    Symbol() = default;
public:
    virtual ~Symbol() = default;
public:
    virtual bool Good(const char & ch) = 0;
};

} //!state

} //!intf

#endif //!INTF_STATE_SYMBOL_H_
