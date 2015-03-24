#ifndef LISTALIASNODE_H
#define LISTALIASNODE_H

#include "executionnode.h"
#include "result/stringresult.h"


class ListAliasNode : public ExecutionNode
{
public:
	ListAliasNode(QMap<QString,QString>* mapAlias);
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
	QMap<QString,QString>* m_mapAlias;
};

#endif // LISTALIASNODE_H
