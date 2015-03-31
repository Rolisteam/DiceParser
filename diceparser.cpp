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

#define DEFAULT_FACES_NUMBER 10

DiceParser::DiceParser()
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



    m_aliasMap = new QMap<QString,QString>;
    m_aliasMap->insert("l5r","D10k");
    m_aliasMap->insert("l5R","D10e10k");
    m_aliasMap->insert("nwod","D10e10c[>7]");
    m_aliasMap->insert("nwod","D10e10c[>7]");

    m_nodeActionMap = new QMap<QString,NodeAction>();
    m_nodeActionMap->insert("@",JumpBackward);


    m_commandList = new QList<QString>();
    m_commandList->append("help");

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
	foreach(QString cmd, m_aliasMap->keys())
	{
		if(str.contains(cmd))
		{
			str.replace(cmd,m_aliasMap->value(cmd));
		}
	}
	return str;
}

bool DiceParser::parseLine(QString str)
{
    m_command = str;
    m_start = new StartingNode();
    ExecutionNode* newNode = NULL;
    m_current = m_start;

	str = convertAlias(str);
    bool keepParsing = readExpression(str,newNode);

    while(keepParsing)
    {
        m_current->setNextNode(newNode);
        m_current = getLatestNode(m_current);
        keepParsing =!str.isEmpty();
        if(keepParsing)
        {
            keepParsing = readOperator(str,m_current);

            m_current = getLatestNode(m_current);
        }
        return true;

    }
    return false;

}

bool DiceParser::readExpression(QString& str,ExecutionNode* & node)
{
    //    int myNumber = 1;
    //    bool hasReadNumber=false;
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
    /*k			case JumpBackward:
            {
                JumpBackwardNode* jumpNode = new JumpBackwardNode();
                previous->setNextNode(jumpNode);
                node = jumpNode;
                isFine=true;
            }*/
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
//    qDebug() << "node count"<< nodeCount;
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
QString DiceParser::getLastDiceResult()
{
    ExecutionNode* next = getLeafNode();
    QString str;
    QTextStream stream(&str);
    Result* result=next->getResult();
    QString dieValue("D%1 : {%2} ");
    while(NULL!=result)
    {
        if(result->hasResultOfType(Result::DICE_LIST))
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

                                resulStr+=QString("%1,").arg(i);
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

        result = result->getPrevious();
    }

    return str.simplified();
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
                qDebug() << list;
                ListSetRollNode* lsrNode = new ListSetRollNode();
                lsrNode->setListValue(list);
                node = lsrNode;
                return true;
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
       node = new HelpNode();
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
        ExecutionNode* nodeExec = new  DiceRollerNode(DEFAULT_FACES_NUMBER);
        ExecutionNode* nodeExec2 = nodeExec;
        bool readOptionSuccessed=false;
        while(readOption(str,nodeExec))
        {
           nodeExec = getLatestNode(nodeExec);
           readOptionSuccessed = true;
        }
        if(readOptionSuccessed)
        {
            previous->setNextNode(nodeExec2);
        }
        else
        {
            delete nodeExec;
            nodeExec = NULL;
            nodeExec2 = NULL;
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
bool DiceParser::readOption(QString& str,ExecutionNode* previous, bool hasDice)
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
                        previous = addRollDiceNode(10,previous);
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
                        previous = addRollDiceNode(10,previous);
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

            }
                break;
            case Explosing:
            {
                Validator* validator = m_parsingToolbox->readValidator(str);
                if(NULL!=validator)
                {
                    if(m_parsingToolbox->isValidValidator(previous,validator))
                    {
                        m_errorMap.insert(ExecutionNode::ENDLESS_LOOP_ERROR,tr("this condition introduce an endless loop: %1. Please, change it").arg(validator->toString()))
                    }
                    ExploseDiceNode* explosedNode = new ExploseDiceNode();
                    explosedNode->setValidator(validator);
                    previous->setNextNode(explosedNode);
                    node = explosedNode;
                    isFine = true;

                }
            }


            }
        }
    }
    return isFine;
}

QList<ExecutionNode::ERROR_CODE>  DiceParser::getErrorList()
{
	return m_start->getErrorList();
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
}

bool DiceParser::readOperand(QString& str,ExecutionNode* & node)
{
    int myNumber=1;

    if(m_parsingToolbox->readNumber(str,myNumber))
    {
        NumberNode* myNumberNode = new NumberNode();
        myNumberNode->setNumber(myNumber);

        node = myNumberNode;
        return true;
    }
        return false;
}
void DiceParser::displayDotTree()
{
    QString str("digraph ExecutionTree {\n");
    m_start->generateDotTree(str);
    str.append("}");

    qDebug()<< str;

}
