/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
* http://renaudguezennec.homelinux.org/accueil,3.html                      *
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
ParsingToolBox::~ParsingToolBox()
{
	if(NULL!=m_logicOp)
	{
		delete m_logicOp;
		m_logicOp = NULL;
	}
}
Validator* ParsingToolBox::readValidator(QString& str)
{
    Validator* returnVal=NULL;
    bool expectSquareBrasket=false;
    if((str.startsWith("[")))
    {
        str=str.remove(0,1);
        expectSquareBrasket = true;
    }

    BooleanCondition::LogicOperator myLogicOp = BooleanCondition::Equal;
    //bool hasReadLogicOperator =
    readLogicOperator(str,myLogicOp);
    int value=0;

    if(readNumber(str,value))
    {
        bool isOk = true;
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

    while(i<str.length() && ((str[i].isNumber()) || ( (i==0) && (str[i]=='-'))))
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
bool ParsingToolBox::readList(QString& str,QStringList& list)
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
bool ParsingToolBox::readDiceRange(QString& str,int& start, int& end)
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
