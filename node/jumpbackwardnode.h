#ifndef JUMPBACKWARDNODE_H
#define JUMPBACKWARDNODE_H

#include "executionnode.h"
#include "result/diceresult.h"
class JumpBackwardNode : public ExecutionNode
{
public:
    /**
     * @brief JumpBackwardNode allows to get result from remote node in the execution tree.
     */
	JumpBackwardNode();
    /**
     * @brief run - performs the actions
     * @param previous
     */
	virtual void run(ExecutionNode* previous = NULL);

	/**
	 * @brief toString
	 * @return
	 */
	virtual QString toString() const;
	/**
	 * @brief getPriority
	 * @return
	 */
	virtual qint64 getPriority() const;
private:
    DiceResult* m_diceResult;

};

#endif // JUMPBACKWARDNODE_H
