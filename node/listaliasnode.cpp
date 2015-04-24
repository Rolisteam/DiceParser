#include "listaliasnode.h"

ListAliasNode::ListAliasNode(QList<DiceAlias*>* apAlias)
    : m_aliasList(apAlias)
{
	m_result = new StringResult();
}
void ListAliasNode::run(ExecutionNode* previous )
{
	m_previousNode = previous;
	StringResult* txtResult = dynamic_cast<StringResult*>(m_result);

	if(NULL != previous)
	{
		if(previous->getResult() == NULL)
		{
			txtResult->setText(toString());

		}
		else
		{
			txtResult->setText(previous->getHelp());
		}
		m_result->setPrevious(previous->getResult());
	}

	if(NULL!=m_nextNode)
	{
		m_nextNode->run(this);
	}
}
QString ListAliasNode::toString() const
{
	QString result(QObject::tr("List of Alias:\n"));
    foreach(DiceAlias* key, *m_aliasList)
	{
        result+=QString("%1 : %2\n").arg(key->getCommand()).arg(key->getValue());
	}
	return result;
}

qint64 ListAliasNode::getPriority() const
{
	return 0;
}
