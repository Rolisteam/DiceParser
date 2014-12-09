#include "helpnode.h"

HelpNode::HelpNode()
{
}
void HelpNode::run(ExecutionNode* previous)
{
    if(NULL != previous)
    {

    }
    else
    {

    }
}
QString HelpNode::toString()const
{
    return QObject::tr("see full documentation at: <a href=\"https://github.com/obiwankennedy/DiceParser/blob/master/HelpMe.md\">https://github.com/obiwankennedy/DiceParser/blob/master/HelpMe.md</a>");
}

qint64 HelpNode::getPriority() const
{
    return 0;
}
