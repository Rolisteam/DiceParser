#ifndef REROLLDICENODE_H
#define REROLLDICENODE_H

#include "executionnode.h"
#include "result/diceresult.h"

class ValidatorList;
/**
 * @brief The RerollDiceNode class reroll dice given a condition and replace(or add) the result.
 */
class RerollDiceNode : public ExecutionNode
{
public:
    /**
     * @brief The ReRollMode enum
     */
    enum ReRollMode
    {
        EQUAL,
        LESSER,
        GREATER
    };
    /**
     * @brief RerollDiceNode
     * @param reroll If true reroll the dice only once, otherwise until the condition is false
     */
    RerollDiceNode(bool reroll, bool addingMode);

    /**
     * @brief ~RerollDiceNode
     */
    virtual ~RerollDiceNode();
    /**
     * @brief run
     * @param previous
     */
    virtual void run(ExecutionNode* previous);

    /**
     * @brief setValidator
     */
    virtual void setValidatorList(ValidatorList*);
    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool) const;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const;

    /**
     * @brief getCopy
     * @return
     */
    virtual ExecutionNode* getCopy() const;

    ExecutionNode* getInstruction() const;
    void setInstruction(ExecutionNode* instruction);

private:
    DiceResult* m_diceResult= nullptr;
    ValidatorList* m_validatorList= nullptr;
    ExecutionNode* m_instruction= nullptr;

    const bool m_reroll;
    const bool m_adding;
};

#endif // REROLLDICENODE_H
