#ifndef PATHNAME_MGMT_REC_TREE_H_
#define PATHNAME_MGMT_REC_TREE_H_

#include <cstdint>

namespace pathname
{
namespace mgmt
{
namespace rec
{

class Tree
{
private:
    std::int64_t m_position;
public:
    Tree();
    Tree(const std::int64_t & pos);
public:
    Tree(const Tree & cpy);
    Tree(Tree && mov);
public:
    Tree & operator=(const Tree & cpy);
    Tree & operator=(Tree && mov);
public:
    Tree & operator=(const std::int64_t & pos);
public:
    std::int64_t & operator*();
    const std::int64_t & operator*() const;
};

} //rec

} //!mgmt

} //!pathname

#endif //!PATHNAME_MGMT_REC_TREE_H_
