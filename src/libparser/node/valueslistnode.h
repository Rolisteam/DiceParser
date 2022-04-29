#ifndef VALUESLISTNODE_H
#define VALUESLISTNODE_H

#include "executionnode.h"
#include "result/diceresult.h"

class ValuesListNode : public ExecutionNode
{
public:
    ValuesListNode();

    virtual void run(ExecutionNode* previous= nullptr) override;
    virtual QString toString(bool) const override;
    virtual qint64 getPriority() const override;
    virtual ExecutionNode* getCopy() const override;

    void insertValue(ExecutionNode*);

private:
    std::vector<ExecutionNode*> m_data;
    DiceResult* m_diceResult = nullptr;
};

#endif // VALUESLISTNODE_H
