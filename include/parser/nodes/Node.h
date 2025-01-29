#ifndef NODE_H
#define NODE_H

#include "../../interpreter/NodeVisitor.h"
#include <string>

class Node
{
public:
    Node() = default;
    virtual ~Node() = default;
    virtual std::string toString(int indentLevel = 0) const = 0;
    virtual void accept(NodeVisitor& visitor) const = 0;
};

#endif