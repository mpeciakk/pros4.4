#ifndef GENERIC_TREE_HPP
#define GENERIC_TREE_HPP

#include <Lib/List.hpp>

class GenericTreeNode {
public:
    explicit GenericTreeNode(void* value) : children(new List()), value(value) {}

    List* children;
    void* value;
};

#endif