#ifndef JUMPBACKWARDNODE_H
#define JUMPBACKWARDNODE_H

#include "executionnode.h"

class JumpBackwardNode : public ExecutionNode
{
public:
	JumpBackwardNode();

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


};

#endif // JUMPBACKWARDNODE_H
