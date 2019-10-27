#ifndef TESTNODE_H
#define TESTNODE_H

#include <Qt>

#include "node/executionnode.h"
#include "result/diceresult.h"
#include <utility>
/**
 * @brief The TestNode class replaces any kind of node for test purpose.
 */
class TestNode : public ExecutionNode
{
public:
    /**
     * @brief TestNode builds an instance
     * @param faces, number of faces of dices
     * @param offset, first value of dice.
     */
    TestNode();
    virtual ~TestNode() override;

    /**
     * @brief run - starts to roll dice.
     */
    virtual void run(ExecutionNode*) override;
    /**
     * @brief toString
     * @param wl
     * @return use to generate dot tree;
     */
    virtual QString toString(bool wl) const override;
    /**
     * @brief getPriority
     * @return priority for dice roll: 4 (higher)
     */
    virtual qint64 getPriority() const override;

    virtual ExecutionNode* getCopy() const override;

    void setResult(Result* result);
};

#endif // TESTNODE_H
