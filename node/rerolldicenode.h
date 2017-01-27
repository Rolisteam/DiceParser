#ifndef REROLLDICENODE_H
#define REROLLDICENODE_H


#include "executionnode.h"
#include "result/diceresult.h"
#include "validator.h"
/**
 * @brief The RerollDiceNode class reroll dice given a condition and replace(or add) the result.
 */
class RerollDiceNode : public ExecutionNode
{

public:
	/**
	 * @brief The ReRollMode enum
	 */
    enum ReRollMode {EQUAL,LESSER,GREATER};
	/**
	 * @brief RerollDiceNode
	 */
    RerollDiceNode();

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
    virtual void setValidator(Validator* );
	/**
	 * @brief toString
	 * @return
	 */
    virtual QString toString(bool )const;


	/**
	 * @brief setAddingMode
	 */
    virtual void setAddingMode(bool);
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

private:
    DiceResult* m_diceResult;
    bool m_adding;
    Validator* m_validator;
};

#endif // REROLLDICENODE_H
