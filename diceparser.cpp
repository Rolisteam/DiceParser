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
#include "diceparser.h"
#include <QDebug>
#include <QStringList>
#include <QObject>
#include <QFile>

#include "node/startingnode.h"
#include "node/scalaroperatornode.h"
#include "node/numbernode.h"
#include "node/keepdiceexecnode.h"
#include "node/sortresult.h"
#include "node/countexecutenode.h"
#include "node/rerolldicenode.h"
#include "node/explosedicenode.h"
#include "node/parenthesesnode.h"
#include "node/helpnode.h"
#include "node/jumpbackwardnode.h"
#include "node/listsetrollnode.h"
#include "node/listaliasnode.h"

#define DEFAULT_FACES_NUMBER 10

DiceParser::DiceParser()
	: m_start(NULL)
{
    m_parsingToolbox = new ParsingToolBox();

    m_mapDiceOp = new QMap<QString,DiceOperator>();
    m_mapDiceOp->insert("D",D);
    m_mapDiceOp->insert("L",L);

    m_OptionOp = new QMap<QString,OptionOperator>();
    m_OptionOp->insert(QObject::tr("k"),Keep);
    m_OptionOp->insert(QObject::tr("K"),KeepAndExplose);
    m_OptionOp->insert(QObject::tr("s"),Sort);
    m_OptionOp->insert(QObject::tr("c"),Count);
    m_OptionOp->insert(QObject::tr("r"),Reroll);
    m_OptionOp->insert(QObject::tr("e"),Explosing);
    m_OptionOp->insert(QObject::tr("a"),RerollAndAdd);
    //m_OptionOp->insert(QObject::tr("@"),JumpBackward);


    m_aliasList = new QList<DiceAlias*>();
    /*m_aliasList->append(new DiceAlias("l5r","D10k"));
    m_aliasList->append(new DiceAlias("l5R","D10K"));
    m_aliasList->append(new DiceAlias("nwod","D10e10c[>7]"));
    m_aliasList->append(new DiceAlias("(.*)wod(.*)","\\1d10e[=10]c[>=\\2]-@c[=1]",false));*/

    m_nodeActionMap = new QMap<QString,NodeAction>();
    m_nodeActionMap->insert("@",JumpBackward);

    m_commandList = new QStringList();
    m_commandList->append("help");
    m_commandList->append("la");

}
DiceParser::~DiceParser()
{
	if(NULL!=m_commandList)
	{
		delete m_commandList;
		m_commandList = NULL;
	}
	if(NULL!=m_nodeActionMap)
	{
		delete m_nodeActionMap;
		m_nodeActionMap = NULL;
	}
	if(NULL!=m_OptionOp)
	{
		delete m_OptionOp;
		m_OptionOp = NULL;
	}
	if(NULL!=m_mapDiceOp)
	{
		delete m_mapDiceOp;
		m_mapDiceOp = NULL;
	}
	if(NULL!=m_parsingToolbox)
	{
		delete m_parsingToolbox;
		m_parsingToolbox = NULL;
	}
	if(NULL!=m_aliasList)
	{
		delete m_aliasList;
		m_aliasList = NULL;
	}
	if(NULL!=m_start)
	{
		delete m_start;
		m_start = NULL;
	}
}

ExecutionNode* DiceParser::getLatestNode(ExecutionNode* node)
{
    ExecutionNode* next = node;
    while(NULL != next->getNextNode() )
    {
        next = next->getNextNode();
    }
    return next;
}
QString DiceParser::convertAlias(QString str)
{
    foreach(DiceAlias* cmd, *m_aliasList)
	{
        cmd->resolved(str);
	}
	return str;
}
QList<DiceAlias*>* DiceParser::getAliases()
{
    return m_aliasList;
}
void DiceParser::insertAlias(DiceAlias* dice, int i)
{
    if(i>m_aliasList->size())
    {
        m_aliasList->insert(i, dice);
    }
}

bool DiceParser::parseLine(QString str)
{
    m_errorMap.clear();
	if(NULL!=m_start)
	{
		delete m_start;
		m_start = NULL;
	}
    m_command = str;
    m_start = new StartingNode();
    ExecutionNode* newNode = NULL;
    m_current = m_start;

	str = convertAlias(str);
    bool keepParsing = readExpression(str,newNode);

    if(keepParsing)
    {
        m_current->setNextNode(newNode);
        m_current = getLatestNode(m_current);
        keepParsing =!str.isEmpty();
        if(keepParsing)
        {
            keepParsing = readOperator(str,m_current);

            m_current = getLatestNode(m_current);
        }
    }

    if(m_errorMap.isEmpty())
    {
      return true;
    }
    else
    {
      return false;
    }

}

bool DiceParser::readExpression(QString& str,ExecutionNode* & node)
{
    ExecutionNode* operandNode=NULL;
    if(m_parsingToolbox->readOpenParentheses(str))
    {
        ExecutionNode* internalNode=NULL;
        if(readExpression(str,internalNode))
        {
            ParenthesesNode* parentheseNode  = new ParenthesesNode();
            parentheseNode->setInternelNode(internalNode);
            node = parentheseNode;
            if(m_parsingToolbox->readCloseParentheses(str))
            {

                ExecutionNode* diceNode=NULL;
                if(readDice(str,diceNode))
                {
                    parentheseNode->setNextNode(diceNode);
                }

                return true;
            }


        }
    }
    else if(readOperand(str,operandNode))
    {
        ExecutionNode* diceNode=NULL;
        if(readDice(str,diceNode))
        {
            operandNode->setNextNode(diceNode);
        }
        node = operandNode;

        operandNode= getLatestNode(operandNode);
        while(readOperator(str,operandNode));
    }
    else if(readCommand(str,operandNode))
    {
        node = operandNode;
         return true;
    }
    else if(readNode(str,operandNode))
    {
        node = operandNode;
        return true;
    }
    else
    {
         ExecutionNode* diceNode=NULL;
        if(readDice(str,diceNode))
        {
            NumberNode* numberNode=new NumberNode();
            numberNode->setNumber(1);
            numberNode->setNextNode(diceNode);
            node = numberNode;
        }
        else
        {
            return false;
        }
    }
    return true;
}
bool DiceParser::readNode(QString& str,ExecutionNode* & node)
{
     QString key= str.left(1);
    if(m_nodeActionMap->contains(key))
    {
        JumpBackwardNode* jumpNode = new JumpBackwardNode();
        node = jumpNode;
        str=str.remove(0,1);
        readOption(str,jumpNode,true);
        return true;
    }
    return false;
}

void DiceParser::Start()
{
    m_start->run();
}

QString DiceParser::displayResult()
{

    ExecutionNode* next = m_start;
    int nodeCount=0;
    int resulCount=0;
    while(NULL != next->getNextNode() )
    {
        next = next->getNextNode();
        ++nodeCount;
    }
    //////////////////////////////////
    //
    //  Display
    //
    //////////////////////////////////

    QString str;
    QTextStream stream(&str);
    Result* result=next->getResult();

    QString totalValue("you got %1 ;");
    QString dieValue("D%1 : {%2} ");

    bool scalarDone=false;
    while(NULL!=result)
    {
        ++resulCount;
        if((result->hasResultOfType(Result::SCALAR))&&(!scalarDone))
        {
            stream << totalValue.arg(result->getResult(Result::SCALAR).toReal()) << endl; //.arg(m_command)
            scalarDone=true;
        }
        else if(result->hasResultOfType(Result::DICE_LIST))
        {

            DiceResult* myDiceResult = dynamic_cast<DiceResult*>(result);
            if(NULL!=myDiceResult)
            {

                QString resulStr;
                quint64 face=0;
                foreach(Die* die, myDiceResult->getResultList())
                {
                    if(!die->hasBeenDisplayed())
                    {
                        resulStr+=QString("%1").arg(die->getValue());
                        die->displayed();
                        face = die->getFaces();


                        if(die->hasChildrenValue())
                        {
                            resulStr+=" [";
                            foreach(qint64 i, die->getListValue())
                            {
                                resulStr+=QString("%1 ").arg(i);
                            }
                            resulStr.remove(resulStr.size()-1,1);
                            resulStr+="]";
                        }
                        resulStr+=", ";
                    }
                }
                resulStr.remove(resulStr.size()-2,2);

                if(!resulStr.isEmpty())
                {
                    stream << dieValue.arg(face).arg(resulStr);
                }

            }
        }
        else if(result->hasResultOfType(Result::STRING))
        {
           stream << result->getResult(Result::STRING).toString();
        }

        result = result->getPrevious();
    }

    QTextStream out(stdout);
    out << str << "you rolled: " <<m_command << endl;
    out <<  endl;


    return QString("%1, you rolled:%3").arg(str.simplified()).arg(m_command) ;

//    qDebug() << "result count:" << resulCount;
}
qreal DiceParser::getLastIntegerResult()
{
    ExecutionNode* next = getLeafNode();
    Result* result=next->getResult();
    qreal resultValue;
    bool scalarDone = false;
    while((result!=NULL)&&(!scalarDone))
    {
        if(result->hasResultOfType(Result::SCALAR))
        {
            resultValue = result->getResult(Result::SCALAR).toReal();
            scalarDone=true;
        }
        result=result->getPrevious();
    }
    return resultValue;
}
QString DiceParser::getStringResult()
{
    ExecutionNode* next = getLeafNode();
    QString str;

    Result* result=next->getResult();
    bool found = false;
    while((NULL!=result) && (!found) )
    {
        if(result->hasResultOfType(Result::STRING))
        {
           str = result->getResult(Result::STRING).toString();
           found = true;
        }
        result = result->getPrevious();
    }
    return str;
}
void DiceParser::getLastDiceResult(ExportedDiceResult& diceValues)
{
    ExecutionNode* next = getLeafNode();
    Result* result=next->getResult();

    while(NULL!=result)
    {
        if(result->hasResultOfType(Result::DICE_LIST))
        {
            DiceResult* diceResult = dynamic_cast<DiceResult*>(result);
            if(NULL!=diceResult)
            {
                bool  hasResult = false;
                quint64 face=0;
                ListDiceResult listpair;
                foreach(Die* die, diceResult->getResultList())
                {
                    if(!die->hasBeenDisplayed())
                    {
                        QList<quint64> valuesResult;
                        hasResult=true;
                        valuesResult.append(die->getValue());
                        die->displayed();
                        face = die->getFaces();
                        if(die->hasChildrenValue())
                        {
                            foreach(qint64 i, die->getListValue())
                            {
                                valuesResult.append(i);
                            }
                        }
                        QPair<QList<quint64>,bool> pair(valuesResult,die->isHighlighted());
                        listpair.append(pair);
                    }
                }
                if(!listpair.isEmpty())
                {
                    if(!diceValues.contains(face))
                    {
                        diceValues.insert(face,listpair);
                    }
                    else
                    {
                        ListDiceResult tmp = diceValues.value(face);
                        tmp.append(listpair);
                        diceValues.insert(face,tmp);
                    }

                }
            }
        }

        result = result->getPrevious();
    }
}
QString DiceParser::getDiceCommand()
{
    return m_command;
}

bool DiceParser::hasIntegerResultNotInFirst()
{
    return hasResultOfType(Result::SCALAR,true);
}

bool DiceParser::hasDiceResult()
{
    return hasResultOfType(Result::DICE_LIST);
}
bool DiceParser::hasStringResult()
{
    return hasResultOfType(Result::STRING);
}
bool DiceParser::hasResultOfType(Result::RESULT_TYPE type, bool notthelast)
{
    ExecutionNode* next = getLeafNode();
    Result* result=next->getResult();
    bool scalarDone = false;
    while((result!=NULL)&&(!scalarDone))
    {
        if(result->hasResultOfType(type) && ((!notthelast)||(notthelast && (NULL!=result->getPrevious()))))
        {
            scalarDone=true;
        }
        result=result->getPrevious();
    }
    return scalarDone;
}
qreal DiceParser::getSumOfDiceResult()
{
    ExecutionNode* next = getLeafNode();
    qreal resultValue=0;
    Result* result=next->getResult();
    bool found = false;
    while((NULL!=result)&&(!found))
    {
        if(result->hasResultOfType(Result::DICE_LIST))
        {
            DiceResult* myDiceResult = dynamic_cast<DiceResult*>(result);
            if(NULL!=myDiceResult)
            {
                foreach(Die* die, myDiceResult->getResultList())
                {
                    resultValue+=die->getValue();
                }
                found = true;
            }
        }
        result = result->getPrevious();
    }
    return resultValue;
}

ExecutionNode* DiceParser::getLeafNode()
{
    ExecutionNode* next = m_start;
    while(NULL != next->getNextNode() )
    {
        next = next->getNextNode();
    }
    return next;
}

bool DiceParser::readDice(QString&  str,ExecutionNode* & node)
{
    DiceOperator currentOperator;

    if(readDiceOperator(str,currentOperator))
    {
        int num;
        if(currentOperator==D)
        {
            if(m_parsingToolbox->readNumber(str,num))
            {
                DiceRollerNode* drNode = new DiceRollerNode(num);
                //            dice.m_diceOp = myOperator;
                //            dice.m_faces = num;
                node = drNode;
                ExecutionNode* current = drNode;
                while(readOption(str,current))
                {
                    current = getLatestNode(current);
                }


                return true;
            }
        }
        else if(currentOperator ==L)
        {
            QStringList list;
            if(m_parsingToolbox->readList(str,list))
            {
                ListSetRollNode* lsrNode = new ListSetRollNode();
                lsrNode->setListValue(list);
                node = lsrNode;
                return true;
            }
            else
            {
                 m_errorMap.insert(ExecutionNode::BAD_SYNTAXE,QObject::tr("List is missing after the L operator. Please, add it (e.g : 1L[sword,spear,gun,arrow]"));
            }
        }

    }

    return false;

}
bool DiceParser::readDiceOperator(QString& str,DiceOperator& op)
{
    QStringList listKey = m_mapDiceOp->keys();
    foreach(QString key, listKey)
    {
        if(str.startsWith(key,Qt::CaseInsensitive))
        {
            str=str.remove(0,key.size());
            op = m_mapDiceOp->value(key);
            return true;
        }
    }
    return false;
}
bool DiceParser::readCommand(QString& str,ExecutionNode* & node)
{
    if(m_commandList->contains(str))
    {
		if(str=="help")
		{
       		node = new HelpNode();
		}
		else if(str=="la")
		{
            node = new ListAliasNode(m_aliasList);
		}
       return true;
    }
    return false;
}

bool DiceParser::readDiceExpression(QString& str,ExecutionNode* & node)
{
    bool returnVal=false;

    ExecutionNode* next = NULL;
    if(readDice(str,next))
    {
        ExecutionNode* latest = next;
        while(readOption(str,latest))
        {
            while(NULL!=latest->getNextNode())
            {
                latest = latest->getNextNode();
            }
        }

        node = next;
        returnVal = true;
    }
    else
    {
        returnVal = false;
    }
    return returnVal;
}
bool DiceParser::readInstructionOperator(QChar c)
{
    if(c == ';')
    {
        return true;
    }
    return false;

}

bool DiceParser::readOperator(QString& str,ExecutionNode* previous)
{
    if(str.isEmpty())
    {
        return false;
    }

    ScalarOperatorNode* node = new ScalarOperatorNode();
    if(node->setOperatorChar(str[0]))
    {

        ExecutionNode* nodeExec = NULL;
        str=str.remove(0,1);//removal of one character
        if(readExpression(str,nodeExec))
        {
            node->setInternalNode(nodeExec);
            if(NULL==nodeExec)
            {
				delete node;
                return false;
            }
            if(node->getPriority()>=nodeExec->getPriority())
            {
                node->setNextNode(nodeExec->getNextNode());
                nodeExec->setNextNode(NULL);
            }
            previous->setNextNode(node);

            return true;
        }
		else
		{
			delete node;
		}
    }
    else if(readInstructionOperator(str[0]))
    {
        str=str.remove(0,1);
        delete node;
       ExecutionNode* nodeExec = NULL;
       if(readExpression(str,nodeExec))
       {

          // nodeExec = getLatestNode(nodeExec);

           if(NULL==nodeExec)
           {
               return false;
           }
           previous->setNextNode(nodeExec);

           return true;
       }
    }
    else
    {
        delete node;
        while(readOption(str,previous,false))
        {
           previous = getLatestNode(previous);
        }
    }
    return false;
}
DiceRollerNode* DiceParser::addRollDiceNode(qint64 faces,ExecutionNode* previous)
{
    DiceRollerNode* mydiceRoller= new DiceRollerNode(faces);
    previous->setNextNode(mydiceRoller);
    return mydiceRoller;
}
ExploseDiceNode* DiceParser::addExploseDiceNode(qint64 value,ExecutionNode* previous)
{
    ExploseDiceNode* exploseDiceNode= new ExploseDiceNode();
    BooleanCondition* condition = new BooleanCondition();
    condition->setValue(value);
    condition->setOperator(BooleanCondition::Equal);
    m_parsingToolbox->isValidValidator(previous,condition);
    exploseDiceNode->setValidator(condition);
    previous->setNextNode(exploseDiceNode);
    return exploseDiceNode;
}
bool DiceParser::readOption(QString& str,ExecutionNode* previous, bool hasDice)//,
{
    if(str.isEmpty())
    {
        return false;
    }

    ExecutionNode* node = NULL;
    bool isFine=false;

    for(int i = 0; ((i<m_OptionOp->keys().size())&&(!isFine));++i )
    {
        QString tmp =m_OptionOp->keys().at(i);

        if(str.startsWith(tmp))
        {

            str=str.remove(0,tmp.size());
        //    option = m_OptionOp->value(tmp);
            switch(m_OptionOp->value(tmp))
            {
            case Keep:
            {
                int myNumber=0;
                bool ascending = m_parsingToolbox->readAscending(str);
                if(m_parsingToolbox->readNumber(str,myNumber))
                {
                    if(!hasDice)
                    {
                        previous = addRollDiceNode(DEFAULT_FACES_NUMBER,previous);
                    }
                    node = m_parsingToolbox->addSort(previous,ascending);

                    KeepDiceExecNode* nodeK = new KeepDiceExecNode();
                    nodeK->setDiceKeepNumber(myNumber);

                    node->setNextNode(nodeK);
                    node = nodeK;
                    isFine = true;
                }
            }
                break;
            case KeepAndExplose:
            {
                int myNumber=0;
                bool ascending = m_parsingToolbox->readAscending(str);
                if(m_parsingToolbox->readNumber(str,myNumber))
                {
                    if(!hasDice)
                    {
                        previous = addRollDiceNode(DEFAULT_FACES_NUMBER,previous);
                    }
                    DiceRollerNode* nodeTmp = dynamic_cast<DiceRollerNode*>(previous);

                    previous = addExploseDiceNode(nodeTmp->getFaces(),previous);

                    node = m_parsingToolbox->addSort(previous,ascending);

                    KeepDiceExecNode* nodeK = new KeepDiceExecNode();
                    nodeK->setDiceKeepNumber(myNumber);

                    node->setNextNode(nodeK);
                    node = nodeK;
                    isFine = true;
                }
            }
                break;
            case Sort:
            {
                bool ascending = m_parsingToolbox->readAscending(str);
                node = m_parsingToolbox->addSort(previous,ascending);
                /*if(!hasDice)
                {
                    m_errorMap.insert(ExecutionNode::BAD_SYNTAXE,QObject::tr("Sort Operator does not support default dice. You should add dice command before the s"));
                }*/
                isFine = true;
            }
                break;
            case Count:
            {
                Validator* validator = m_parsingToolbox->readValidator(str);
                if(NULL!=validator)
                {
                    /// @todo display warning here.
                    bool b = m_parsingToolbox->isValidValidator(previous,validator);

                    CountExecuteNode* countNode = new CountExecuteNode();
                    countNode->setValidator(validator);

                    previous->setNextNode(countNode);
                    node = countNode;
                    isFine = true;
                }
                else
                {
                   m_errorMap.insert(ExecutionNode::BAD_SYNTAXE,QObject::tr("Validator is missing after the c operator. Please, change it"));
                }
            }
                break;
            case Reroll:
            case RerollAndAdd:
            {
                Validator* validator = m_parsingToolbox->readValidator(str);
                if(NULL!=validator)
                {
                    /// @todo display warning here.
                    bool b = m_parsingToolbox->isValidValidator(previous,validator);
                    RerollDiceNode* rerollNode = new RerollDiceNode();
                    if(m_OptionOp->value(tmp)==RerollAndAdd)
                    {
                        rerollNode->setAddingMode(true);
                    }
                    rerollNode->setValidator(validator);
                    previous->setNextNode(rerollNode);
                    node = rerollNode;
                    isFine = true;
                }
                else
                {
                   m_errorMap.insert(ExecutionNode::BAD_SYNTAXE,QObject::tr("Validator is missing after the %1 operator. Please, change it").arg(m_OptionOp->value(tmp)==Reroll?"r":"a"));
                }

            }
                break;
            case Explosing:
            {
                Validator* validator = m_parsingToolbox->readValidator(str);
                if(NULL!=validator)
                {
                    if(!m_parsingToolbox->isValidValidator(previous,validator))
                    {
                        m_errorMap.insert(ExecutionNode::ENDLESS_LOOP_ERROR,QObject::tr("This condition %1 introduces an endless loop. Please, change it").arg(validator->toString()));
                    }
                    ExploseDiceNode* explosedNode = new ExploseDiceNode();
                    explosedNode->setValidator(validator);
                    previous->setNextNode(explosedNode);
                    node = explosedNode;
                    isFine = true;

                }
                else
                {
                   m_errorMap.insert(ExecutionNode::BAD_SYNTAXE,QObject::tr("Validator is missing after the e operator. Please, change it"));
                }
            }


            }
        }
    }
    return isFine;
}

QMap<ExecutionNode::ERROR_CODE,QString>  DiceParser::getErrorMap()
{
    return m_start->getExecutionErrorMap();
}
QString DiceParser::humanReadableError()
{
    QMapIterator<ExecutionNode::ERROR_CODE,QString> i(m_errorMap);
    QString str="";
    while (i.hasNext())
    {
        i.next();
        str.append(i.value());
        str.append("\n");
    }

    ///list
    QMapIterator<ExecutionNode::ERROR_CODE,QString> j(getErrorMap());
    while(j.hasNext())
    {
        j.next();
        str.append(j.value());
        str.append("\n");
    }
    return str;
}

bool DiceParser::readOperand(QString& str,ExecutionNode* & node)
{
    int myNumber=1;

    if(m_parsingToolbox->readNumber(str,myNumber))
    {
		NumberNode* numberNode = new NumberNode();
		numberNode->setNumber(myNumber);

		node = numberNode;
        return true;
    }
        return false;
}
void DiceParser::writeDownDotTree(QString filepath)
{
    QString str("digraph ExecutionTree {\n");
    m_start->generateDotTree(str);
    str.append("}");


    QFile file(filepath);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream in(&file);
        in << str;
    }

    //qDebug()<< str;

}
