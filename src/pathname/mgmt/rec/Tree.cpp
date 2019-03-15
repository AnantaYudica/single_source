#include "pathname/mgmt/rec/Tree.h"

#include <utility>

using namespace std;
using namespace pathname::mgmt::rec;


Tree::Tree() :
    m_position(-1)
{}

Tree::Tree(const std::int64_t & pos) :
    m_position(pos)
{}

Tree::Tree(const Tree & cpy) :
    m_position(cpy.m_position)
{}

Tree::Tree(Tree && mov) :
    m_position(mov.m_position)
{
    mov.m_position = -1;
}

Tree & Tree::operator=(const Tree & cpy)
{
    m_position = cpy.m_position;
    return *this;
}

Tree & Tree::operator=(Tree && mov)
{
    m_position = mov.m_position;
    mov.m_position = -1;
    return *this;
}

Tree & Tree::operator=(const std::int64_t & pos)
{
    m_position = pos;
    return *this;
}

std::int64_t & Tree::operator*()
{
    return m_position;
}

const std::int64_t & Tree::operator*() const
{
    return m_position;
}
