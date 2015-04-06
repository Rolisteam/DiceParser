#ifndef LISTALIASNODE_H
#define LISTALIASNODE_H

#include "executionnode.h"
#include "result/stringresult.h"
#include "dicealias.h"

class ListAliasNode : public ExecutionNode
{
public:
    ListAliasNode(QList<DiceAlias*>* mapAlias);
	/**
	  * @brief run
	  * @param previous
	  */
	virtual void run(ExecutionNode* previous = NULL);

	/**
	 * @brief toString
	 * @return
	 */
	virtual QString toString()const;
	/**
	 * @brief getPriority
	 * @return
	 */
	virtual qint64 getPriority() const;

private:
    QList<DiceAlias*>* m_aliasList;
};

#endif // LISTALIASNODE_H
