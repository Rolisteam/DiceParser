/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
* http://www.rolisteam.org/contact                      *
*                                                                          *
*  This file is part of DiceParser                                         *
*                                                                          *
* DiceParser is free software; you can redistribute it and/or modify       *
* it under the terms of the GNU General Public License as published by     *
* the Free Software Foundation; either version 2 of the License, or        *
* (at your option) any later version.                                      *
*                                                                          *
* This program is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with this program; if not, write to the                            *
* Free Software Foundation, Inc.,                                          *
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.                 *
***************************************************************************/
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


    m_logicOperation = new QMap<QString,CompositeValidator::LogicOperation>();
    m_logicOperation->insert("|",CompositeValidator::OR);
    m_logicOperation->insert("^",CompositeValidator::EXCLUSIVE_OR);
    m_logicOperation->insert("&",CompositeValidator::AND);

    m_conditionOperation = new QMap<QString,OperationCondition::ConditionOperator>();
    m_conditionOperation->insert("%",OperationCondition::Modulo);


    m_arithmeticOperation = new QHash<QString,ScalarOperatorNode::ArithmeticOperator>();
    m_arithmeticOperation->insert(QStringLiteral("+"),ScalarOperatorNode::PLUS);
    m_arithmeticOperation->insert(QStringLiteral("-"),ScalarOperatorNode::MINUS);
    m_arithmeticOperation->insert(QStringLiteral("*"),ScalarOperatorNode::MULTIPLICATION);
    m_arithmeticOperation->insert(QStringLiteral("x"),ScalarOperatorNode::MULTIPLICATION);
    m_arithmeticOperation->insert(QStringLiteral("/"),ScalarOperatorNode::DIVIDE);
    m_arithmeticOperation->insert(QStringLiteral("÷"),ScalarOperatorNode::DIVIDE);

}

ParsingToolBox::ParsingToolBox(const ParsingToolBox& data)
{

}
ParsingToolBox::~ParsingToolBox()
{
    if(NULL!=m_logicOp)
    {
        delete m_logicOp;
        m_logicOp = NULL;
    }
}
ExecutionNode* ParsingToolBox::addSort(ExecutionNode* e,bool b)
{
    SortResultNode* nodeSort = new SortResultNode();
    nodeSort->setSortAscending(b);
    e->setNextNode(nodeSort);
    return nodeSort;
}
bool ParsingToolBox::readDiceLogicOperator(QString& str,OperationCondition::ConditionOperator& op)
{
    QString longKey;
    foreach(QString tmp, m_conditionOperation->keys())
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
        op = m_conditionOperation->value(longKey);
        return true;
    }

    return false;
}

bool ParsingToolBox::readArithmeticOperator(QString &str, ScalarOperatorNode::ArithmeticOperator &op)
{
    bool found = false;
    //QHash<QString,ScalarOperatorNode::ArithmeticOperator>::Iterator

    for(auto i = m_arithmeticOperation->begin() ; i !=m_arithmeticOperation->end() && !found; ++i)
    {
        if(str.startsWith(i.key()))
        {
                op = i.value();
                str=str.remove(0,i.key().size());
                found=true;
        }
    }
    return found;
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
    BooleanCondition::LogicOperator myLogicOp = BooleanCondition::Equal;
    /*bool hasReadLogicOperator = */readLogicOperator(str,myLogicOp);


    OperationCondition::ConditionOperator condiOp = OperationCondition::Modulo;
    bool hasDiceLogicOperator = readDiceLogicOperator(str,condiOp);
    qint64 value=0;

    if(hasDiceLogicOperator)
    {
        if(readNumber(str,value))
        {
            OperationCondition* condition = new OperationCondition();
            condition->setValue(value);
            Validator* valid = readValidator(str);
            BooleanCondition* boolC = dynamic_cast<BooleanCondition*>(valid);
            if(NULL!=boolC)
            {
                condition->setBoolean(boolC);
            }
            returnVal = condition;
        }

    }
    else if(readNumber(str,value))
    {
        if(str.startsWith("-"))
        {
            str=str.remove(0,1);
            qint64 end=0;
            if(readNumber(str,end))
            {
                str=str.remove(0,1);
                Range* range = new Range();
                range->setValue(value,end);
                returnVal = range;
            }
        }
        else
        {
            BooleanCondition* condition = new BooleanCondition();
            condition->setValue(value);
            condition->setOperator(myLogicOp);
            returnVal = condition;
        }
    }
    return returnVal;
}
Validator* ParsingToolBox::readCompositeValidator(QString& str)
{
    bool expectSquareBrasket=false;
    if((str.startsWith("[")))
    {
        str=str.remove(0,1);
        expectSquareBrasket = true;
    }

    Validator* tmp = readValidator(str);
    CompositeValidator::LogicOperation opLogic;

    QVector<CompositeValidator::LogicOperation>* operators = new QVector<CompositeValidator::LogicOperation>();
    QList<Validator*>* validatorList = new QList<Validator*>();

    while(NULL!=tmp)
    {
        bool hasOperator = readLogicOperation(str,opLogic);
        if( hasOperator )
        {
            operators->append(opLogic);
            validatorList->append(tmp);
            tmp = readValidator(str);
        }
        else
        {
            if((expectSquareBrasket)&&(str.startsWith("]")))
            {
                str=str.remove(0,1);
                //isOk=true;
            }

            if(!validatorList->isEmpty())
            {
                validatorList->append(tmp);
            }
            else
            {
                return tmp;
            }
            tmp = NULL;
        }

    }
    if(!validatorList->isEmpty())
    {
        CompositeValidator* validator = new CompositeValidator();
        validator->setOperationList(operators);
        validator->setValidatorList(validatorList);
        return validator;
    }
    else
    {
        return NULL;
    }
}
bool ParsingToolBox::readLogicOperation(QString& str,CompositeValidator::LogicOperation& op)
{
    QString longKey;
    foreach(QString tmp, m_logicOperation->keys())
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
        op = m_logicOperation->value(longKey);
        return true;
    }

    return false;
}

bool ParsingToolBox::readNumber(QString& str, qint64& myNumber)
{
    if(str.isEmpty())
        return false;

    QString number;
    int i=0;
    while(i<str.length() && ((str[i].isNumber()) || ( (i==0) && (str[i]=='-'))))
    {
        number+=str[i];
        ++i;
    }

    if(number.isEmpty())
        return false;

    bool ok;
    myNumber = number.toLongLong(&ok);
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
bool ParsingToolBox::readList(QString& str,QStringList& list,QList<Range>& ranges)
{
    if(str.startsWith("["))
    {
        str=str.remove(0,1);
        int pos = str.lastIndexOf("]");
        if(-1!=pos)
        {
            QString liststr = str.left(pos);
            list = liststr.split(",");
			str=str.remove(0,pos+1);
            readProbability(list,ranges);
            return true;
        }
    }
    return false;
}
bool ParsingToolBox::readAscending(QString& str)
{
    if(str.isEmpty())
    {
        return false;
    }
    else if(str.at(0)=='l')
    {
        str=str.remove(0,1);
        return true;
    }
    return false;
}
bool ParsingToolBox::isValidValidator(ExecutionNode* previous, Validator* val)
{
    DiceRollerNode* node = getDiceRollerNode(previous);
    if(NULL!=node)
    {
        return (val->getValidRangeSize(node->getFaces())<node->getFaces());
    }
    else
    {
        return true;
    }
}
DiceRollerNode* ParsingToolBox::getDiceRollerNode(ExecutionNode* previous)
{
    while(NULL!=previous)
    {
        DiceRollerNode* node = dynamic_cast<DiceRollerNode*>(previous);
        if(NULL!=node)
        {
            return node;
        }
        previous = previous->getPreviousNode();
    }
}
bool ParsingToolBox::readDiceRange(QString& str,qint64& start, qint64& end)
{
    bool expectSquareBrasket=false;

    if((str.startsWith("[")))
    {
        str=str.remove(0,1);
        expectSquareBrasket = true;
    }
    if(readNumber(str,start))
    {
        if(str.startsWith("-"))
        {
            str=str.remove(0,1);
            if(readNumber(str,end))
            {
                if(expectSquareBrasket)
                {
                    if(str.startsWith("]"))
                    {
                        str=str.remove(0,1);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            else
            {
               return false;
            }
        }
    }

}
ParsingToolBox::LIST_OPERATOR  ParsingToolBox::readListOperator(QString& str)
{

    if(str.startsWith('u'))
    {
        return UNIQUE;
    }
    return NONE;
}

void ParsingToolBox::readPainterParameter(PainterNode* painter,QString& str)
{
    if(str.startsWith('['))
    {
        str=str.remove(0,1);
        int pos = str.indexOf(']');

        if(pos>-1)
        {

            QString data = str.left(pos);
            str=str.remove(0,pos+1);
            QStringList duos = data.split(',');
            foreach(QString duoStr,duos)
            {
                QStringList keyValu = duoStr.split(':');
                if(keyValu.size()==2)
                {
                    painter->insertColorItem(keyValu[1],keyValu[0].toInt());
                }
            }
        }
    }
}

void ParsingToolBox::readProbability(QStringList& str,QList<Range>& ranges)
{
    quint64 totalDistance=0;
    quint64 undefDistance = 0;
    int undefCount=0;
    int maxValue = 0;
    int i=0;
    int j=0;
    //range
    foreach(QString line,str)
    {
        int pos = line.indexOf('[');
        if(-1!=pos)
        {
            QString range = line.right(line.length()-pos);
            line = line.left(pos);
            str[j]=line;
            qint64 start;
            qint64 end;
            if(readDiceRange(range,start,end))
            {
                Range range;
                range.setValue(start,end);
                ranges.append(range);
                totalDistance += end-start+1;
                ++i;
            }
            else
            {
                Range range;
                range.setStart(start);
                ranges.append(range);
                ++undefCount;
                undefDistance +=start;
            }
            if((end>maxValue)||(i==1))
            {
                maxValue = end;
            }
        }
        ++j;

    }

    qint64 totalDistPourcent = totalDistance * undefDistance / (100-undefDistance);

    if(totalDistPourcent<undefCount)
    {
        totalDistPourcent = undefCount;
    }

    for(int i = 0; i< ranges.size(); ++i)
    {
        Range tmp = ranges.at(i);
        if(!tmp.isFullyDefined())
        {
            int dist  = tmp.getStart();
            tmp.setStart(maxValue+1);
            maxValue+=1;
            double truc = undefDistance*1.0/dist;

            tmp.setEnd(maxValue+(truc*totalDistPourcent));
            maxValue = maxValue+(truc*totalDistPourcent);
            ranges[i]=tmp;
        }
    }



}
