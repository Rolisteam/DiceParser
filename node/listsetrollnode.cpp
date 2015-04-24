#include "listsetrollnode.h"
#include "die.h"

ListSetRollNode::ListSetRollNode()
    :m_diceResult(new DiceResult()),m_stringResult(new StringResult())
{
    m_result = m_stringResult;
}
ListSetRollNode::~ListSetRollNode()
{
	if(NULL!=m_diceResult)
	{
		delete m_diceResult;
		m_diceResult =NULL;
	}
}

QStringList ListSetRollNode::getList()
{
    return m_values;
}
QString ListSetRollNode::toString() const
{
    return QString("ListSetRollNode_").arg(m_values.join(','));
}
qint64 ListSetRollNode::getPriority() const
{
    qint64 priority=4;
//    if(NULL!=m_nextNode)
//    {
//        priority = m_nextNode->getPriority();
//    }


    return priority;
}
void ListSetRollNode::run(ExecutionNode* previous)
{
    m_previousNode = previous;
    if(NULL!=previous)
    {
        Result* result=previous->getResult();
        if(NULL!=result)
        {
            quint64 diceCount = result->getResult(Result::SCALAR).toReal();
            m_result->setPrevious(result);
            QStringList rollResult;
            for(quint64 i=0; i < diceCount ; ++i)
            {
                Die* die = new Die();
                die->setFaces(m_values.size());
                die->roll();
                m_diceResult->insertResult(die);
                if(die->getValue()-1<m_values.size())
                {
                    rollResult << m_values[die->getValue()-1];
                }
            }
            m_stringResult->setText(rollResult.join(","));
            if(NULL!=m_nextNode)
            {
                m_nextNode->run(this);
            }
        }
    }



}
void ListSetRollNode::setListValue(QStringList lirs)
{
    m_values = lirs;
}
