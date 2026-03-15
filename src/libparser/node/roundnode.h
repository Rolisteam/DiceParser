#ifndef ROUNDNODE_H
#define ROUNDNODE_H

#include "executionnode.h"
#include "scalarresult.h"

class RoundNode : public ExecutionNode
{
public:
    enum Mode
    {
        FLOOR,
        CEIL,
        ROUND
    };
    RoundNode(Mode mode);

    // ExecutionNode interface
public:
    void run(ExecutionNode* previous);
    qint64 getPriority() const;
    ExecutionNode* getCopy() const;
    void setCommand(ExecutionNode* cmd);

private:
    std::unique_ptr<ScalarResult> m_scalarResult;
    ExecutionNode* m_cmd= nullptr;
    Mode m_mode{ROUND};
};

#endif // ROUNDNODE_H
