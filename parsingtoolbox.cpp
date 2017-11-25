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


QHash<QString,QString>*  ParsingToolBox::m_variableHash = nullptr;

ParsingToolBox::ParsingToolBox()
    : m_logicOp(new QMap<QString,BooleanCondition::LogicOperator>()),
      m_logicOperation(new QMap<QString,CompositeValidator::LogicOperation>()),
      m_conditionOperation(new QMap<QString,OperationCondition::ConditionOperator>()),
      m_arithmeticOperation(new QHash<QString,Die::ArithmeticOperator>())
{
    //m_logicOp = ;
    m_logicOp->insert(">=",BooleanCondition::GreaterOrEqual);
    m_logicOp->insert("<=",BooleanCondition::LesserOrEqual);
    m_logicOp->insert("<",BooleanCondition::LesserThan);
    m_logicOp->insert("=",BooleanCondition::Equal);
    m_logicOp->insert(">",BooleanCondition::GreaterThan);
    m_logicOp->insert("!=",BooleanCondition::Different);


    //m_logicOperation = ;
    m_logicOperation->insert("|",CompositeValidator::OR);
    m_logicOperation->insert("^",CompositeValidator::EXCLUSIVE_OR);
    m_logicOperation->insert("&",CompositeValidator::AND);

   // m_conditionOperation = ;
    m_conditionOperation->insert("%",OperationCondition::Modulo);


    //m_arithmeticOperation = new QHash<QString,ScalarOperatorNode::ArithmeticOperator>();
    m_arithmeticOperation->insert(QStringLiteral("+"),Die::PLUS);
    m_arithmeticOperation->insert(QStringLiteral("-"),Die::MINUS);
    m_arithmeticOperation->insert(QStringLiteral("*"),Die::MULTIPLICATION);
    m_arithmeticOperation->insert(QStringLiteral("x"),Die::MULTIPLICATION);
    m_arithmeticOperation->insert(QStringLiteral("/"),Die::DIVIDE);
    m_arithmeticOperation->insert(QStringLiteral("÷"),Die::DIVIDE);

}

ParsingToolBox::ParsingToolBox(const ParsingToolBox& data)
{

}
ParsingToolBox::~ParsingToolBox()
{
    if(nullptr!=m_logicOp)
    {
        delete m_logicOp;
        m_logicOp = nullptr;
    }
    if(nullptr != m_logicOperation)
    {
        delete m_logicOperation;
        m_logicOperation= nullptr;
    }
    if(nullptr != m_conditionOperation)
    {
        delete m_conditionOperation;
        m_conditionOperation= nullptr;
    }
    if(nullptr != m_arithmeticOperation)
    {
        delete m_arithmeticOperation;
        m_arithmeticOperation= nullptr;
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

bool ParsingToolBox::readArithmeticOperator(QString &str, Die::ArithmeticOperator &op)
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
    Validator* returnVal=nullptr;
    BooleanCondition::LogicOperator myLogicOp = BooleanCondition::Equal;
    readLogicOperator(str,myLogicOp);


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
            if(nullptr!=boolC)
            {
                condition->setBoolean(boolC);
            }
            returnVal = condition;
        }

    }
    else if(readNumber(str,value))
    {
        bool isRange = false;
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
                isRange = true;
            }
            else
            {
                str.prepend("-");
            }
        }

        if(!isRange)
        {
            BooleanCondition* condition = new BooleanCondition();
            condition->setValue(value);
            condition->setOperator(myLogicOp);
            returnVal = condition;
        }
    }
    return returnVal;
}
IfNode::ConditionType ParsingToolBox::readConditionType(QString& str)
{
    IfNode::ConditionType type = IfNode::OnEach;
    if(str.startsWith('.'))
    {
        str=str.remove(0,1);
        type = IfNode::OneOfThem;
    }
    else if(str.startsWith('*'))
    {
        str=str.remove(0,1);
        type = IfNode::AllOfThem;
    }
    else if(str.startsWith(':'))
    {
        str=str.remove(0,1);
        type = IfNode::OnScalar;
    }
    return type;
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

    while(nullptr!=tmp)
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
            tmp = nullptr;
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
        return nullptr;
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
    {
        QString reason;
        return readVariable(str,myNumber,reason);
    }

    bool ok;
    myNumber = number.toLongLong(&ok);
    if(ok)
    {
        str=str.remove(0,number.size());
        return true;
    }

    return false;
}

bool ParsingToolBox::readString(QString &str, QString& strResult)
{
    if(str.isEmpty())
        return false;


    if(str.startsWith('"'))
    {
        str=str.remove(0,1);

    int i=0;
    int j=0;
    bool previousEscape=false;
    QString result;
    /*&&
            (((!previousEscape) && !(str[i]=='"')) || (previousEscape) && !(str[i]=='"'))
                || (str[i]=='\\'))*/
    while(i<str.length() && (!(!previousEscape && (str[i]=='"'))  || (previousEscape && str[i]!='"')))
    {
        if(str[i]=='\\')
        {
            previousEscape = true;
        }
        else
        {
            if(previousEscape && str[i]!='\"')
            {
                result += '\\';
                ++j;
            }
            result+=str[i];
            previousEscape = false;
        }
        ++i;
    }

    if(!result.isEmpty())
    {
        str=str.remove(0,i);
        strResult = result;
        if(str.startsWith('"'))
        {
            str=str.remove(0,1);
            return true;
        }
    }
    }

    return false;
}

bool ParsingToolBox::readVariable(QString &str, qint64 &myNumber, QString& reasonFail)
{
    if(str.isEmpty())
        return false;
    if(str.startsWith("${"))
    {
        str=str.remove(0,2);
    }
    QString key;
    int post = str.indexOf('}');
    key = str.left(post);

    if(nullptr!=m_variableHash)
    {
        //qDebug() << m_variableHash->keys();
        if(m_variableHash->contains(key))
        {
            QString value = m_variableHash->value(key);
            bool ok;
            int valueInt = value.toInt(&ok);
            if(ok)
            {
                myNumber = valueInt;
                str=str.remove(0,post+1);
                return true;
            }
            else
            {
                reasonFail = QStringLiteral("Variable value is %1, not a number").arg(value);
            }

        }
        else
        {
            reasonFail = QStringLiteral("Variable not found");
        }
    }
    else
    {
        reasonFail = QStringLiteral("No Variables are defined");
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
        int pos = str.indexOf("]");
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
    if(nullptr!=node)
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
    while(nullptr!=previous)
    {
        DiceRollerNode* node = dynamic_cast<DiceRollerNode*>(previous);
        if(nullptr!=node)
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
                }
            }
        }
    }
    return false;

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

QHash<QString, QString> *ParsingToolBox::getVariableHash()
{
    return m_variableHash;
}

void ParsingToolBox::setVariableHash(QHash<QString, QString> *variableHash)
{
    m_variableHash = variableHash;
}

void ParsingToolBox::readProbability(QStringList& str,QList<Range>& ranges)
{
    quint64 totalDistance=0;
    quint64 undefDistance = 0;
    int undefCount=0;
    int maxValue = 0;
    int i=0;
    int j=0;
    bool hasPercentage=false;
    //QList<Range> rangesTemp;
    //range
    for(QString line:str)
    {
        int pos = line.indexOf('[');
        if(-1!=pos)
        {
            QString rangeStr = line.right(line.length()-pos);
            line = line.left(pos);
            str[j]=line;
            qint64 start;
            qint64 end;
            if(readDiceRange(rangeStr,start,end))
            {
                Range range;
                range.setValue(start,end);
                ranges.append(range);
                totalDistance += end-start+1;
                ++i;
            }
            else//pourcentage
            {
                hasPercentage = true;
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
        else
        {
                Range range;
                range.setEmptyRange(true);
                ranges.append(range);
        }
        ++j;

    }

    if((hasPercentage)&&(undefDistance!=0))
    {
        qreal ratio = (qreal)100.0 / (qreal)undefDistance;
        qint64 realStart=0;
        for(int i = 0; i< ranges.size(); ++i)
        {
            Range tmp = ranges.at(i);
            if(!tmp.isFullyDefined())
            {
                int dist  = tmp.getStart();
                tmp.setStart(realStart+1);
                double truc = dist*ratio;

                tmp.setEnd(realStart+truc);
                realStart = tmp.getEnd();
                //qDebug() <<"start:"<< tmp.getStart() << "end:"<< realStart;
                ranges[i]=tmp;
            }
        }
    }
    else
    {
        int limitUp = 1;
        for(int i = 0; i< ranges.size(); ++i)
        {
            Range range = ranges.at(i);
            if(range.isEmptyRange())
            {
                range.setStart(limitUp);
                range.setEnd(limitUp);
                range.setEmptyRange(false);

            }
            else
            {
                qint64 sizeRange = range.getEnd()-range.getStart();
                range.setStart(limitUp);
                limitUp+=sizeRange+1;
                range.setEnd(limitUp);
            }
            ++limitUp;
            ranges[i]=range;
        }

    }

}
bool ParsingToolBox::readComment(QString& str, QString & result, QString& comment)
{
    QString left = str;
    str = str.trimmed();
    if(str.startsWith("#"))
    {
        comment = left;
        str = str.remove(0,1);
        result = str.trimmed();
        return true;
    }
    return false;
}
