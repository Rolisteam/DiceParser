#ifndef EXECUTIONNODE_H
#define EXECUTIONNODE_H

#include "result.h"
#include <QDebug>
class ExecutionNode
{
public:
    enum ERROR_CODE {NO_ERROR,DIE_RESULT_EXPECTED,BAD_SYNTAXE};
    ExecutionNode();
    virtual ~ExecutionNode();
    virtual void run(ExecutionNode* previous = NULL)=0;
    Result* getResult();
    void setNextNode(ExecutionNode*);
    ExecutionNode* getNextNode();
    virtual QString toString()const=0;
    virtual qint64 getPriority() const=0;
    virtual QList<ExecutionNode::ERROR_CODE> getErrorList();
protected:
    Result* m_result;
    ExecutionNode* m_nextNode;
    QList<ExecutionNode::ERROR_CODE> m_errors;
};

#endif // EXECUTIONNODE_H
