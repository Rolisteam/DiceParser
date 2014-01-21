#include <QString>

#include "parsingtoolbox.h"
#include "node/sortresult.h"


ParsingToolBox::ParsingToolBox()
{
    m_logicOp = new QMap<QString,BooleanCondition::LogicOperator>();
    m_logicOp->insert(">=",BooleanCondition::GreaterOrEqual);
    m_logicOp->insert("<=",BooleanCondition::LesserOrEqual);
    m_logicOp->insert("<",BooleanCondition::LesserThan);
    m_logicOp->insert("=",BooleanCondition::Equal);
    m_logicOp->insert(">",BooleanCondition::GreaterThan);
}
ExecutionNode* ParsingToolBox::addSort(ExecutionNode* e,bool b)
{
    SortResultNode* nodeSort = new SortResultNode();
    nodeSort->setSortAscending(b);
    e->setNextNode(nodeSort);
    return nodeSort;
}
bool ParsingToolBox::readLogicOperator(QString& str,BooleanCondition::LogicOperator& op)
{
    QString longKey;
    foreach(QString tmp, m_logicOp->keys())
    {
        if(str.startsWith(tmp))
        {
            if(longKey.size()<tmp.size())
            {
                longKey = tmp;
            }
        }
    }
    if(longKey.size()>0)
    {
        str=str.remove(0,longKey.size());
        op = m_logicOp->value(longKey);
        return true;
    }

    return false;
}
Validator* ParsingToolBox::readValidator(QString& str)
{
    Validator* returnVal=NULL;
    bool expectSquareBrasket=false;
    bool isOk = true;
    if((str.startsWith("[")))
    {
        str=str.remove(0,1);
        expectSquareBrasket = true;
    }

    BooleanCondition::LogicOperator myLogicOp = BooleanCondition::Equal;
    bool hasReadLogicOperator = readLogicOperator(str,myLogicOp);
    int value=0;

    if(readNumber(str,value))
    {
        if(str.startsWith("-"))
        {
            str=str.remove(0,1);
            int end=0;
            if(readNumber(str,end))
            {
                if(expectSquareBrasket)
                {
                    if(str.startsWith("]"))
                    {
                        str=str.remove(0,1);
                        isOk=true;
                    }
                    else
                    {
                        isOk=false;
                    }
                }
                if(isOk)
                {
                    str=str.remove(0,1);
                    Range* range = new Range();
                    range->setValue(value,end);
                    returnVal = range;
                }
            }
        }
        else
        {
            if((expectSquareBrasket)&&(str.startsWith("]")))
            {
                str=str.remove(0,1);
                isOk=true;
            }
            if(isOk)
            {
                BooleanCondition* condition = new BooleanCondition();
                condition->setValue(value);
                condition->setOperator(myLogicOp);
                returnVal = condition;
            }
        }
    }
    return returnVal;
}
bool ParsingToolBox::readNumber(QString& str, int& myNumber)
{
    if(str.isEmpty())
        return false;

    QString number;
    int i=0;

    while(i<str.length() && str[i].isNumber())
    {
        number+=str[i];
        ++i;
    }

    if(number.isEmpty())
        return false;

    bool ok;
    myNumber = number.toInt(&ok);
    if(ok)
    {
        str=str.remove(0,number.size());
        return true;
    }
    return false;
}
bool ParsingToolBox::readOpenParentheses(QString& str)
{
    if(str.startsWith("("))
    {
        str=str.remove(0,1);
           return true;
    }
    else
        return false;
}
bool ParsingToolBox::readCloseParentheses(QString& str)
{
    if(str.startsWith(")"))
    {
        str=str.remove(0,1);
           return true;
    }
    else
        return false;
}
