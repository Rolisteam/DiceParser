#include "helpnode.h"

HelpNode::HelpNode()
{
    m_result = new StringResult();
}
void HelpNode::run(ExecutionNode* previous)
{
	m_previousNode = previous;
    StringResult* txtResult = dynamic_cast<StringResult*>(m_result);

    qDebug() << m_result->hasResultOfType(Result::SCALAR) << m_result->hasResultOfType(Result::STRING);
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
QString HelpNode::toString()const
{
    return QObject::tr("Rolisteam Dice Parser: Full documentation at: https://github.com/obiwankennedy/DiceParser/blob/master/HelpMe.md\n");
}

qint64 HelpNode::getPriority() const
{
    return 0;
}
