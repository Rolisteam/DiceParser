#include "helpnode.h"

HelpNode::HelpNode()
    : m_path("https://github.com/obiwankennedy/DiceParser/blob/master/HelpMe.md")
{
    m_result = new StringResult();
}
void HelpNode::run(ExecutionNode* previous)
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
QString HelpNode::toString(bool wl) const
{
	if(wl)
	{
		return QString("%1 [label=\"Rolisteam Dice Parser:\nFull documentation at: %2\"]").arg(m_id).arg(m_path);
	}
	else
	{
		return m_id;
	}
}

qint64 HelpNode::getPriority() const
{
    return 0;
}
void HelpNode::setHelpPath(QString path)
{
    m_path = path;
}
