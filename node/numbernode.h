#ifndef NUMBERNODE_H
#define NUMBERNODE_H
#include "node/executionnode.h"

class NumberNode : public ExecutionNode
{
public:
    NumberNode();
    void run(ExecutionNode* previous);
    void setNumber(qint64);

private:
    qint64 m_number;

};

#endif // NUMBERNODE_H
