#ifndef FORLOOPNODE_H
#define FORLOOPNODE_H

#include "executionnode.h"
#include "result/diceresult.h"
#include <memory>

class MockNode : public ExecutionNode
{
public:
    MockNode();
    void run(ExecutionNode* node);
    void setResult(Result* result);
    QString toString(bool withLabel) const;
    qint64 getPriority() const;
    ExecutionNode* getCopy() const;
};

class ForLoopNode : public ExecutionNode
{
public:
    ForLoopNode();
    void run(ExecutionNode* previous);

    void setInternal(ExecutionNode* internal);

    QString toString(bool withLabel) const;
    qint64 getPriority() const;
    ExecutionNode* getCopy() const;

private:
    std::unique_ptr<ExecutionNode> m_internal;
    DiceResult* m_diceResult;
};

#endif // FORLOOPNODE_H
