#ifndef OCCURENCECOUNTNODE_H
#define OCCURENCECOUNTNODE_H

#include "executionnode.h"
#include "validator.h"

class OccurenceCountNode : public ExecutionNode
{
public:
    OccurenceCountNode();

    void run(ExecutionNode* previous = nullptr);
    virtual QString toString(bool withLabel)const;

    ExecutionNode* getCopy() const;
    qint64 getPriority() const;

private:
    qint64 m_width=0;
    qint64 m_height=0;
    Validator* m_validator;
};

#endif // OCCURENCECOUNTNODE_H
