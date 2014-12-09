#include "helpnode.h"

HelpNode::HelpNode()
{
    m_result = new StringResult();
}
void HelpNode::run(ExecutionNode* previous)
{
    StringResult* txtResult = dynamic_cast<StringResult*>(m_result);
    if(NULL != previous)
    {

        txtResult->setText(toString());
    }
    else
    {
        txtResult->setText(previous->getHelp());
    }
}
QString HelpNode::toString()const
{
    return QObject::tr("Rolisteam Dice Parser: Full documentation at: <a href=\"https://github.com/obiwankennedy/DiceParser/blob/master/HelpMe.md\">https://github.com/obiwankennedy/DiceParser/blob/master/HelpMe.md</a>");
}

qint64 HelpNode::getPriority() const
{
    return 0;
}
