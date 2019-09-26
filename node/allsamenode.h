#ifndef ALLSAMENODE_H
#define ALLSAMENODE_H

#include "executionnode.h"

#include "result/diceresult.h"
#include "validator.h"

class AllSameNode : public ExecutionNode
{
public:
    AllSameNode();
//    virtual ~AllSameNode();

    virtual void run(ExecutionNode* previous);
    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool withLabel) const;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const;

    virtual ExecutionNode* getCopy() const;

private:
    DiceResult* m_diceResult;
};

#endif // FILTERNODE_H
