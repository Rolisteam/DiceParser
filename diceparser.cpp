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
#include "diceparser.h"
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QStringList>
#include <functional>

#include "node/bind.h"
#include "node/countexecutenode.h"
#include "node/explodedicenode.h"
#include "node/filternode.h"
#include "node/groupnode.h"
#include "node/helpnode.h"
#include "node/ifnode.h"
#include "node/jumpbackwardnode.h"
#include "node/keepdiceexecnode.h"
#include "node/listaliasnode.h"
#include "node/listsetrollnode.h"
#include "node/mergenode.h"
#include "node/numbernode.h"
#include "node/occurencecountnode.h"
#include "node/paintnode.h"
#include "node/parenthesesnode.h"
#include "node/rerolldicenode.h"
#include "node/scalaroperatornode.h"
#include "node/sortresult.h"
#include "node/splitnode.h"
#include "node/startingnode.h"
#include "node/stringnode.h"
#include "node/uniquenode.h"
#include "node/valueslistnode.h"
#include "node/variablenode.h"

#define DEFAULT_FACES_NUMBER 10

DiceParser::DiceParser()
//: //m_start(nullptr),m_current(nullptr)
{
    m_currentTreeHasSeparator= false;
    m_parsingToolbox= new ParsingToolBox();
    ParsingToolBox::setStartNodes(&m_startNodes);

    m_mapDiceOp= new QMap<QString, DiceOperator>();
    m_mapDiceOp->insert(QStringLiteral("D"), D);
    m_mapDiceOp->insert(QStringLiteral("L"), L);

    m_OptionOp= new QMap<QString, OptionOperator>();
    m_OptionOp->insert(QStringLiteral("k"), Keep);
    m_OptionOp->insert(QStringLiteral("K"), KeepAndExplode);
    m_OptionOp->insert(QStringLiteral("s"), Sort);
    m_OptionOp->insert(QStringLiteral("c"), Count);
    m_OptionOp->insert(QStringLiteral("r"), Reroll);
    m_OptionOp->insert(QStringLiteral("e"), Explode);
    m_OptionOp->insert(QStringLiteral("R"), RerollUntil);
    m_OptionOp->insert(QStringLiteral("a"), RerollAndAdd);
    m_OptionOp->insert(QStringLiteral("m"), Merge);
    m_OptionOp->insert(QStringLiteral("i"), ifOperator);
    m_OptionOp->insert(QStringLiteral("p"), Painter);
    m_OptionOp->insert(QStringLiteral("f"), Filter);
    m_OptionOp->insert(QStringLiteral("y"), Split);
    m_OptionOp->insert(QStringLiteral("u"), Unique);
    m_OptionOp->insert(QStringLiteral("g"), Group);
    m_OptionOp->insert(QStringLiteral("b"), Bind);
    m_OptionOp->insert(QStringLiteral("o"), Occurences);

    m_aliasList= new QList<DiceAlias*>();

    m_nodeActionMap= new QMap<QString, NodeAction>();
    m_nodeActionMap->insert(QStringLiteral("@"), JumpBackward);

    m_commandList= new QStringList();
    m_commandList->append(QStringLiteral("help"));
    m_commandList->append(QStringLiteral("la"));
}
DiceParser::~DiceParser()
{
    if(nullptr != m_commandList)
    {
        delete m_commandList;
        m_commandList= nullptr;
    }
    if(nullptr != m_nodeActionMap)
    {
        delete m_nodeActionMap;
        m_nodeActionMap= nullptr;
    }
    if(nullptr != m_OptionOp)
    {
        delete m_OptionOp;
        m_OptionOp= nullptr;
    }
    if(nullptr != m_mapDiceOp)
    {
        delete m_mapDiceOp;
        m_mapDiceOp= nullptr;
    }
    if(nullptr != m_parsingToolbox)
    {
        delete m_parsingToolbox;
        m_parsingToolbox= nullptr;
    }
    if(nullptr != m_aliasList)
    {
        delete m_aliasList;
        m_aliasList= nullptr;
    }
    if(nullptr != m_start)
    {
        delete m_start;
        m_start= nullptr;
    }
}

QString DiceParser::convertAlias(QString str)
{
    for(auto& cmd : *m_aliasList)
    {
        if(cmd->isEnable())
        {
            cmd->resolved(str);
        }
    }
    return str;
}
QList<DiceAlias*>* DiceParser::getAliases()
{
    return m_aliasList;
}
void DiceParser::insertAlias(DiceAlias* dice, int i)
{
    if(i >= m_aliasList->size())
    {
        m_aliasList->insert(i, dice);
    }
}

bool DiceParser::parseLine(QString str, bool allowAlias)
{
    m_errorMap.clear();
    m_comment= QString("");
    if(!m_startNodes.empty())
    {
        qDeleteAll(m_startNodes);
        m_startNodes.clear();
    }
    m_currentTreeHasSeparator= false;
    if(allowAlias)
    {
        str= convertAlias(str);
    }
    m_command= str;
    bool hasInstruction= readInstructionList(str);
    bool value= hasInstruction;
    if(!hasInstruction)
    {
        m_errorMap.insert(
            ExecutionNode::NOTHING_UNDERSTOOD,
            QObject::tr("Nothing was understood. To roll dice: !1d6 - full documation: "
                        "<a "
                        "href=\"https://github.com/Rolisteam/DiceParser/blob/master/HelpMe.md\">https://github.com/"
                        "Rolisteam/DiceParser/blob/master/HelpMe.md</a>"));
    }
    else if(hasInstruction && !str.isEmpty())
    {
        auto i= m_command.size() - str.size();
        m_warningMap.insert(
            ExecutionNode::UNEXPECTED_CHARACTER,
            QObject::tr("Unexpected character at %1 - end of command was ignored \"%2\"").arg(i).arg(str));
    }
    if(!m_errorMap.isEmpty())
        value= false;

    return value;
}

bool DiceParser::readExpression(QString& str, ExecutionNode*& node)
{
    ExecutionNode* operandNode= nullptr;
    if(m_parsingToolbox->readOpenParentheses(str))
    {
        ExecutionNode* internalNode= nullptr;
        if(readExpression(str, internalNode))
        {
            ParenthesesNode* parentheseNode= new ParenthesesNode();
            parentheseNode->setInternelNode(internalNode);
            node= parentheseNode;
            if(m_parsingToolbox->readCloseParentheses(str))
            {
                ExecutionNode* diceNode= nullptr;
                ExecutionNode* operatorNode= nullptr;
                if(readDice(str, diceNode))
                {
                    parentheseNode->setNextNode(diceNode);
                }
                else if(readExpression(str, operatorNode))
                {
                    parentheseNode->setNextNode(operatorNode);
                }
                return true;
            }
            else
            {
                m_warningMap.insert(ExecutionNode::BAD_SYNTAXE,
                                    QObject::tr("Expected closing parenthesis - can't validate the inside."));
            }
        }
    }
    else if(m_parsingToolbox->readOperand(str, operandNode))
    {
        ExecutionNode* diceNode= nullptr;
        if(readDice(str, diceNode))
        {
            operandNode->setNextNode(diceNode);
        }
        node= operandNode;

        operandNode= ParsingToolBox::getLatestNode(operandNode);
        // ExecutionNode* operatorNode=nullptr;
        while(readOperator(str, operandNode))
        {
            // operandNode->setNextNode(operatorNode);
            operandNode= ParsingToolBox::getLatestNode(operandNode);
        };
        return true;
    }
    else if(readCommand(str, operandNode))
    {
        node= operandNode;
        return true;
    }
    else if(readNode(str, operandNode))
    {
        node= operandNode;
        return true;
    }
    else if(readOptionFromNull(str, operandNode))
    {
        node= operandNode;
        return true;
    }
    else if(readOperatorFromNull(str, operandNode))
    {
        node= operandNode;
        return true;
    }
    else if(readValuesList(str, operandNode))
    {
        node= operandNode;
        return true;
    }
    else
    {
        ExecutionNode* diceNode= nullptr;
        if(readDice(str, diceNode))
        {
            NumberNode* numberNode= new NumberNode();
            numberNode->setNumber(1);
            numberNode->setNextNode(diceNode);
            node= numberNode;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

bool DiceParser::readOptionFromNull(QString& str, ExecutionNode*& node)
{
    StartingNode nodePrevious;
    if(readOption(str, &nodePrevious))
    {
        auto nodeNext= nodePrevious.getNextNode();
        nodePrevious.setNextNode(nullptr);
        node= nodeNext;
        return true;
    }
    return false;
}

bool DiceParser::readOperatorFromNull(QString& str, ExecutionNode*& node)
{
    StartingNode nodePrevious;
    if(readOperator(str, &nodePrevious))
    {
        auto nodeNext= nodePrevious.getNextNode();
        nodePrevious.setNextNode(nullptr);
        node= nodeNext;
        return true;
    }
    return false;
}

bool DiceParser::readValuesList(QString& str, ExecutionNode*& node)
{
    if(str.startsWith("["))
    {
        str= str.remove(0, 1);
        int pos= ParsingToolBox::findClosingCharacterIndexOf('[', ']', str, 1); // str.indexOf("]");
        if(-1 != pos)
        {
            QString liststr= str.left(pos);
            auto list= liststr.split(",");
            str= str.remove(0, pos + 1);
            auto values= new ValuesListNode();
            for(auto var : list)
            {
                qint64 number= 1;
                QString error;
                if(ParsingToolBox::readDynamicVariable(var, number))
                {
                    VariableNode* variableNode= new VariableNode();
                    variableNode->setIndex(number - 1);
                    variableNode->setData(&m_startNodes);
                    values->insertValue(variableNode);
                }
            }
            node= values;
            return true;
        }
    }
    return false;
}

bool DiceParser::readNode(QString& str, ExecutionNode*& node)
{
    if(str.isEmpty())
        return false;

    QString key= str.at(0);
    if(m_nodeActionMap->contains(key))
    {
        JumpBackwardNode* jumpNode= new JumpBackwardNode();
        node= jumpNode;
        str= str.remove(0, 1);
        readOption(str, jumpNode);
        return true;
    }
    return false;
}

void DiceParser::start()
{
    for(auto start : m_startNodes)
    {
        start->run();
    }
}

QList<qreal> DiceParser::getLastIntegerResults()
{
    QList<qreal> resultValues;
    for(auto node : m_startNodes)
    {
        ExecutionNode* next= getLeafNode(node);
        Result* result= next->getResult();
        bool scalarDone= false;
        while((result != nullptr) && (!scalarDone))
        {
            if(result->hasResultOfType(Result::SCALAR))
            {
                resultValues << result->getResult(Result::SCALAR).toReal();
                scalarDone= true;
            }
            result= result->getPrevious();
        }
    }
    return resultValues;
}
QStringList DiceParser::getStringResult()
{
    QStringList stringListResult;
    for(auto node : m_startNodes)
    {
        ExecutionNode* next= getLeafNode(node);
        QString str;
        Result* result= next->getResult();
        bool found= false;
        while((nullptr != result) && (!found))
        {
            if(result->hasResultOfType(Result::STRING))
            {
                str= result->getResult(Result::STRING).toString();
                found= true;
            }
            result= result->getPrevious();
        }
        stringListResult << str;
    }
    return stringListResult;
}
QStringList DiceParser::getAllStringResult(bool& hasAlias)
{
    // QStringList allResult;
    QStringList stringListResult;
    for(auto node : m_startNodes)
    {
        ExecutionNode* next= getLeafNode(node);
        Result* result= next->getResult();

        while(nullptr != result)
        {
            if(result->hasResultOfType(Result::STRING))
            {
                StringResult* stringResult= dynamic_cast<StringResult*>(result);
                if(nullptr != stringResult)
                {
                    stringListResult << stringResult->getText();
                    hasAlias= stringResult->hasHighLight();
                }
            }
            result= result->getPrevious();
        }
    }
    return stringListResult;
}
QStringList DiceParser::getAllDiceResult(bool& hasAlias)
{
    QStringList stringListResult;
    for(auto node : m_startNodes)
    {
        ExecutionNode* next= getLeafNode(node);
        Result* result= next->getResult();
        QList<Die*> dieListResult;

        while(nullptr != result)
        {
            if(result->hasResultOfType(Result::DICE_LIST))
            {
                DiceResult* stringResult= dynamic_cast<DiceResult*>(result);
                if(nullptr != stringResult)
                {
                    for(auto& die : stringResult->getResultList())
                    {
                        if(!dieListResult.contains(die))
                        {
                            dieListResult.removeAll(die);
                            dieListResult << die;
                        }
                    }
                    hasAlias= true;
                }
            }
            result= result->getPrevious();
        }
        for(auto& die : dieListResult)
        {
            if(die->isHighlighted())
            {
                for(qint64& value : die->getListValue())
                {
                    stringListResult << QString::number(value);
                }
            }
        }
    }

    return stringListResult;
}

void DiceParser::getDiceResultFromAllInstruction(QList<ExportedDiceResult>& resultList)
{
    for(auto start : m_startNodes)
    {
        ExecutionNode* next= getLeafNode(start);
        Result* result= next->getResult();
        // qDebug() << "result id" << result->toString(true);
        ExportedDiceResult nodeResult;
        while(nullptr != result)
        {
            if(result->hasResultOfType(Result::DICE_LIST))
            {
                DiceResult* diceResult= dynamic_cast<DiceResult*>(result);
                QList<HighLightDice> list;
                quint64 faces= 0;

                for(auto& die : diceResult->getResultList())
                {
                    faces= die->getFaces();
                    // qDebug() << "face" << faces;
                    HighLightDice hlDice(die->getListValue(), die->isHighlighted(), die->getColor(),
                                         die->hasBeenDisplayed(), die->getFaces());
                    list.append(hlDice);
                }
                nodeResult.insert(faces, list);
            }
            result= result->getPrevious();
        }
        resultList.append(nodeResult);
    }
    // qDebug() << resultList.size();
}

void DiceParser::getLastDiceResult(QList<ExportedDiceResult>& diceValuesList, bool& homogeneous)
{
    for(auto start : m_startNodes)
    {
        ExportedDiceResult diceValues;
        ExecutionNode* next= getLeafNode(start);
        Result* result= next->getResult();
        while(nullptr != result)
        {
            if(result->hasResultOfType(Result::DICE_LIST))
            {
                DiceResult* diceResult= dynamic_cast<DiceResult*>(result);
                if(nullptr != diceResult)
                {
                    if(homogeneous)
                    {
                        homogeneous= diceResult->isHomogeneous();
                    }
                    quint64 face= 0;
                    ListDiceResult listpair;
                    for(auto& die : diceResult->getResultList())
                    {
                        if(!die->hasBeenDisplayed())
                        {
                            QList<qint64> valuesResult;
                            valuesResult.append(die->getValue());
                            die->displayed();
                            face= die->getFaces();
                            if(die->hasChildrenValue())
                            {
                                for(qint64& i : die->getListValue())
                                {
                                    valuesResult.append(i);
                                }
                            }
                            HighLightDice hlDice(valuesResult, die->isHighlighted(), die->getColor(),
                                                 die->hasBeenDisplayed(), 0);
                            listpair.append(hlDice);
                        }
                    }
                    if(!listpair.isEmpty())
                    {
                        if(!diceValues.contains(face))
                        {
                            diceValues.insert(face, listpair);
                        }
                        else
                        {
                            ListDiceResult tmp= diceValues.value(face);
                            tmp.append(listpair);
                            diceValues.insert(face, tmp);
                        }
                    }
                }
            }
            result= result->getPrevious();
        }
        if(!diceValues.isEmpty())
        {
            diceValuesList.append(diceValues);
        }
    }
}
QString DiceParser::getDiceCommand() const
{
    return m_command;
}

bool DiceParser::hasIntegerResultNotInFirst()
{
    bool result= false;
    for(auto node : m_startNodes)
    {
        result|= hasResultOfType(Result::SCALAR, node);
    }
    return result;
}

bool DiceParser::hasDiceResult()
{
    bool result= false;
    for(auto node : m_startNodes)
    {
        result|= hasResultOfType(Result::DICE_LIST, node);
    }
    return result;
}
bool DiceParser::hasStringResult()
{
    bool result= false;
    for(auto node : m_startNodes)
    {
        result|= hasResultOfType(Result::STRING, node);
    }
    return result;
}
bool DiceParser::hasResultOfType(Result::RESULT_TYPE type, ExecutionNode* node, bool notthelast)
{
    bool scalarDone= false;
    ExecutionNode* next= getLeafNode(node);
    Result* result= next->getResult();
    while((result != nullptr) && (!scalarDone))
    {
        if(result->hasResultOfType(type) && ((!notthelast) || (nullptr != result->getPrevious())))
        {
            scalarDone= true;
        }
        result= result->getPrevious();
    }
    return scalarDone;
}
QList<qreal> DiceParser::getSumOfDiceResult()
{
    QList<qreal> resultValues;
    for(auto node : m_startNodes)
    {
        qreal resultValue= 0;
        ExecutionNode* next= getLeafNode(node);
        Result* result= next->getResult();
        bool found= false;
        while((nullptr != result) && (!found))
        {
            if(result->hasResultOfType(Result::DICE_LIST))
            {
                DiceResult* myDiceResult= dynamic_cast<DiceResult*>(result);
                if(nullptr != myDiceResult)
                {
                    for(auto& die : myDiceResult->getResultList())
                    {
                        resultValue+= die->getValue();
                    }
                    found= true;
                }
            }
            result= result->getPrevious();
        }
        resultValues << resultValue;
    }
    return resultValues;
}
int DiceParser::getStartNodeCount() const
{
    return static_cast<int>(m_startNodes.size());
}
ExecutionNode* DiceParser::getLeafNode(ExecutionNode* start)
{
    ExecutionNode* next= start;
    while(nullptr != next->getNextNode())
    {
        next= next->getNextNode();
    }
    return next;
}

bool DiceParser::readDice(QString& str, ExecutionNode*& node)
{
    DiceOperator currentOperator;

    if(readDiceOperator(str, currentOperator))
    {
        if(currentOperator == D)
        {
            qint64 max;
            qint64 min;
            bool unique= (ParsingToolBox::UNIQUE == m_parsingToolbox->readListOperator(str)) ? true : false;
            Die::ArithmeticOperator op;

            bool hasOp= m_parsingToolbox->readArithmeticOperator(str, op);
            if(m_parsingToolbox->readNumber(str, max))
            {
                if(max < 1)
                {
                    m_errorMap.insert(
                        ExecutionNode::BAD_SYNTAXE,
                        QObject::tr("Dice with %1 face(s) does not exist. Please, put a value higher than 0").arg(max));
                    return false;
                }
                DiceRollerNode* drNode= new DiceRollerNode(max);
                drNode->setUnique(unique);
                if(hasOp)
                {
                    drNode->setOperator(op);
                }
                node= drNode;
                ExecutionNode* current= drNode;
                while(readOption(str, current))
                {
                    current= ParsingToolBox::getLatestNode(current);
                }
                return true;
            }
            else if(m_parsingToolbox->readDiceRange(str, min, max))
            {
                DiceRollerNode* drNode= new DiceRollerNode(max, min);
                drNode->setUnique(unique);
                if(hasOp)
                {
                    drNode->setOperator(op);
                }
                node= drNode;
                ExecutionNode* current= drNode;
                while(readOption(str, current))
                {
                    current= ParsingToolBox::getLatestNode(current);
                }
                return true;
            }
        }
        else if(currentOperator == L)
        {
            QStringList list;
            QList<Range> listRange;
            ParsingToolBox::LIST_OPERATOR op= m_parsingToolbox->readListOperator(str);
            if(m_parsingToolbox->readList(str, list, listRange))
            {
                ListSetRollNode* lsrNode= new ListSetRollNode();
                lsrNode->setRangeList(listRange);
                if(op == ParsingToolBox::UNIQUE)
                {
                    lsrNode->setUnique(true);
                }
                lsrNode->setListValue(list);
                node= lsrNode;
                return true;
            }
            else
            {
                m_errorMap.insert(
                    ExecutionNode::BAD_SYNTAXE,
                    QObject::tr(
                        "List is missing after the L operator. Please, add it (e.g : 1L[sword,spear,gun,arrow])"));
            }
        }
    }

    return false;
}
bool DiceParser::readDiceOperator(QString& str, DiceOperator& op)
{
    QStringList listKey= m_mapDiceOp->keys();
    for(const QString& key : listKey)
    {
        if(str.startsWith(key, Qt::CaseInsensitive))
        {
            str= str.remove(0, key.size());
            op= m_mapDiceOp->value(key);
            return true;
        }
    }
    return false;
}
bool DiceParser::readCommand(QString& str, ExecutionNode*& node)
{
    if(m_commandList->contains(str))
    {
        if(str == QLatin1String("help"))
        {
            str= str.remove(0, QLatin1String("help").size());
            HelpNode* help= new HelpNode();
            if(!m_helpPath.isEmpty())
            {
                help->setHelpPath(m_helpPath);
            }
            node= help;
        }
        else if(str == QLatin1String("la"))
        {
            str= str.remove(0, QLatin1String("la").size());
            node= new ListAliasNode(m_aliasList);
        }
        return true;
    }
    return false;
}

bool DiceParser::readDiceExpression(QString& str, ExecutionNode*& node)
{
    bool returnVal= false;

    ExecutionNode* next= nullptr;
    if(readDice(str, next))
    {
        ExecutionNode* latest= next;
        while(readOption(str, latest))
        {
            while(nullptr != latest->getNextNode())
            {
                latest= latest->getNextNode();
            }
        }

        node= next;
        returnVal= true;
    }
    else
    {
        returnVal= false;
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
bool DiceParser::readInstructionList(QString& str)
{
    if(str.isEmpty())
        return false;

    bool hasInstruction= false;
    bool readInstruction= true;
    while(readInstruction)
    {
        ExecutionNode* startNode= nullptr;
        bool keepParsing= readExpression(str, startNode);
        if(nullptr != startNode)
        {
            hasInstruction= true;
            m_startNodes.push_back(startNode);
            auto latest= startNode;
            if(keepParsing)
            {
                latest= ParsingToolBox::getLatestNode(latest);
                keepParsing= !str.isEmpty();
                while(keepParsing)
                {
                    auto before= str;
                    if(readOperator(str, latest))
                    {
                        latest= ParsingToolBox::getLatestNode(latest);
                    }
                    keepParsing= (!str.isEmpty() && (before != str));
                }
            }
            if(!str.isEmpty() && readInstructionOperator(str[0]))
            {
                str= str.remove(0, 1);
            }
            else
            {
                QString result;
                QString comment;
                if(m_parsingToolbox->readComment(str, result, comment))
                {
                    m_command.remove(comment);
                    m_comment= result;
                }
                readInstruction= false;
            }
        }
        else
        {
            readInstruction= false;
        }
    }
    return hasInstruction;
}

bool DiceParser::readOperator(QString& str, ExecutionNode* previous)
{
    if(str.isEmpty() || nullptr == previous)
    {
        return false;
    }

    Die::ArithmeticOperator op;
    if(m_parsingToolbox->readArithmeticOperator(str, op))
    {
        ScalarOperatorNode* node= new ScalarOperatorNode();
        node->setArithmeticOperator(op);
        ExecutionNode* nodeExec= nullptr;
        if(readExpression(str, nodeExec))
        {
            node->setInternalNode(nodeExec);
            if(nullptr == nodeExec)
            {
                delete node;
                return false;
            }
            ExecutionNode* nodeExecOrChild= nodeExec;
            ExecutionNode* parent= nullptr;

            while((nullptr != nodeExecOrChild) && (node->getPriority() < nodeExecOrChild->getPriority()))
            {
                parent= nodeExecOrChild;
                nodeExecOrChild= nodeExecOrChild->getNextNode();
            }
            // management of operator priority
            if((nullptr != nodeExecOrChild) && (nodeExec != nodeExecOrChild))
            {
                // good 1 1 2 ; bad 1 0 4
                if(nodeExecOrChild->getPriority() >= node->getPriority())
                {
                    node->setNextNode(nodeExecOrChild);
                    parent->setNextNode(nullptr);
                }
            }
            else if(node->getPriority() >= nodeExec->getPriority())
            {
                node->setNextNode(nodeExec->getNextNode());
                nodeExec->setNextNode(nullptr);
            }

            // nodeResult = node;
            previous->setNextNode(node);

            return true;
        }
        else
        {
            delete node;
        }
    }
    else
    {
        while(readOption(str, previous))
        {
            previous= ParsingToolBox::getLatestNode(previous);
        }
    }
    return false;
}
bool DiceParser::hasSeparator() const
{
    return m_currentTreeHasSeparator;
}
DiceRollerNode* DiceParser::addRollDiceNode(qint64 faces, ExecutionNode* previous)
{
    DiceRollerNode* mydiceRoller= new DiceRollerNode(faces);
    previous->setNextNode(mydiceRoller);
    return mydiceRoller;
}
ExplodeDiceNode* DiceParser::addExplodeDiceNode(qint64 value, ExecutionNode* previous)
{
    ExplodeDiceNode* explodeDiceNode= new ExplodeDiceNode();
    NumberNode* node= new NumberNode();
    node->setNumber(value);
    BooleanCondition* condition= new BooleanCondition();
    condition->setValueNode(node);
    condition->setOperator(BooleanCondition::Equal);
    m_parsingToolbox->isValidValidator(previous, condition);
    explodeDiceNode->setValidator(condition);
    previous->setNextNode(explodeDiceNode);
    return explodeDiceNode;
}
bool DiceParser::readOption(QString& str, ExecutionNode* previous) //,
{
    if(str.isEmpty())
    {
        return false;
    }

    ExecutionNode* node= nullptr;
    bool found= false;
    auto keys= m_OptionOp->keys();
    for(int i= 0; ((i < keys.size()) && (!found)); ++i)
    {
        QString key= keys.at(i);

        if(str.startsWith(key))
        {
            str= str.remove(0, key.size());
            auto operatorName= m_OptionOp->value(key);
            switch(operatorName)
            {
            case Keep:
            {
                qint64 myNumber= 0;
                bool ascending= m_parsingToolbox->readAscending(str);

                if(m_parsingToolbox->readNumber(str, myNumber))
                {
                    node= m_parsingToolbox->addSort(previous, ascending);
                    KeepDiceExecNode* nodeK= new KeepDiceExecNode();
                    nodeK->setDiceKeepNumber(myNumber);
                    node->setNextNode(nodeK);
                    node= nodeK;
                    found= true;
                }
            }
            break;
            case KeepAndExplode:
            {
                qint64 myNumber= 0;
                bool ascending= m_parsingToolbox->readAscending(str);
                if(m_parsingToolbox->readNumber(str, myNumber))
                {
                    /* if(!hasDice)
                        {
                            previous = addRollDiceNode(DEFAULT_FACES_NUMBER,previous);
                        }*/
                    DiceRollerNode* nodeTmp= dynamic_cast<DiceRollerNode*>(previous);
                    if(nullptr != nodeTmp)
                    {
                        previous= addExplodeDiceNode(static_cast<qint64>(nodeTmp->getFaces()), previous);
                    }

                    node= m_parsingToolbox->addSort(previous, ascending);

                    KeepDiceExecNode* nodeK= new KeepDiceExecNode();
                    nodeK->setDiceKeepNumber(myNumber);

                    node->setNextNode(nodeK);
                    node= nodeK;
                    found= true;
                }
            }
            break;
            case Filter:
            {
                Validator* validator= m_parsingToolbox->readCompositeValidator(str);
                if(nullptr != validator)
                {
                    m_parsingToolbox->isValidValidator(previous, validator);

                    FilterNode* filterNode= new FilterNode();
                    filterNode->setValidator(validator);

                    previous->setNextNode(filterNode);
                    node= filterNode;
                    found= true;
                }
            }
            break;
            case Sort:
            {
                bool ascending= m_parsingToolbox->readAscending(str);
                node= m_parsingToolbox->addSort(previous, ascending);
                /*if(!hasDice)
                    {
                        m_errorMap.insert(ExecutionNode::BAD_SYNTAXE,QObject::tr("Sort Operator does not support default
                   dice. You should add dice command before the s"));
                    }*/
                found= true;
            }
            break;
            case Count:
            {
                Validator* validator= m_parsingToolbox->readCompositeValidator(str);
                if(nullptr != validator)
                {
                    m_parsingToolbox->isValidValidator(previous, validator);

                    CountExecuteNode* countNode= new CountExecuteNode();
                    countNode->setValidator(validator);

                    previous->setNextNode(countNode);
                    node= countNode;
                    found= true;
                }
                else
                {
                    m_errorMap.insert(ExecutionNode::BAD_SYNTAXE,
                                      QObject::tr("Validator is missing after the c operator. Please, change it"));
                }
            }
            break;
            case Reroll:
            case RerollUntil:
            case RerollAndAdd:
                // Todo: I think that Exploding and Rerolling could share the same code
                {
                    Validator* validator= m_parsingToolbox->readCompositeValidator(str);
                    if(nullptr != validator)
                    {
                        if(!m_parsingToolbox->isValidValidator(previous, validator))
                        {
                            m_errorMap.insert(
                                ExecutionNode::BAD_SYNTAXE,
                                QObject::tr("Validator is missing after the %1 operator. Please, change it")
                                    .arg(operatorName == Reroll ? "r" : "a"));
                        }

                        auto reroll= (operatorName == RerollAndAdd || operatorName == Reroll);
                        auto addingMode= (operatorName == RerollAndAdd);
                        RerollDiceNode* rerollNode= new RerollDiceNode(reroll, addingMode);
                        ExecutionNode* nodeParam= nullptr;
                        if(readParameterNode(str, nodeParam))
                        {
                            rerollNode->setInstruction(nodeParam);
                        }
                        rerollNode->setValidator(validator);
                        previous->setNextNode(rerollNode);
                        node= rerollNode;
                        found= true;
                    }
                    else
                    {
                        m_errorMap.insert(ExecutionNode::BAD_SYNTAXE,
                                          QObject::tr("Validator is missing after the %1 operator. Please, change it")
                                              .arg(operatorName == Reroll ?
                                                       QStringLiteral("r") :
                                                       operatorName == RerollUntil ?
                                                       QStringLiteral("R") :
                                                       operatorName == RerollAndAdd ? QStringLiteral("a") : ""));
                    }
                }
                break;
            case Explode:
            {
                Validator* validator= m_parsingToolbox->readCompositeValidator(str);
                if(nullptr != validator)
                {
                    if(!m_parsingToolbox->isValidValidator(previous, validator))
                    {
                        m_errorMap.insert(ExecutionNode::ENDLESS_LOOP_ERROR,
                                          QObject::tr("This condition %1 introduces an endless loop. Please, change it")
                                              .arg(validator->toString()));
                    }
                    ExplodeDiceNode* explodedNode= new ExplodeDiceNode();
                    explodedNode->setValidator(validator);
                    previous->setNextNode(explodedNode);
                    node= explodedNode;
                    found= true;
                }
                else
                {
                    m_errorMap.insert(ExecutionNode::BAD_SYNTAXE,
                                      QObject::tr("Validator is missing after the e operator. Please, change it"));
                }
            }
            break;
            case Merge:
            {
                MergeNode* mergeNode= new MergeNode();
                mergeNode->setStartList(&m_startNodes);
                previous->setNextNode(mergeNode);
                node= mergeNode;
                found= true;
            }
            break;
            case Bind:
            {
                BindNode* bindNode= new BindNode();
                bindNode->setStartList(&m_startNodes);
                previous->setNextNode(bindNode);
                node= bindNode;
                found= true;
            }
            break;
            case Occurences:
            {
                qint64 number= 0;
                auto occNode= new OccurenceCountNode();
                if(m_parsingToolbox->readNumber(str, number))
                {
                    occNode->setWidth(number);
                    Validator* validator= m_parsingToolbox->readCompositeValidator(str);
                    if(validator)
                    {
                        occNode->setValidator(validator);
                    }
                    else if(m_parsingToolbox->readComma(str))
                    {
                        if(m_parsingToolbox->readNumber(str, number))
                        {
                            occNode->setHeight(number);
                        }
                    }
                }
                previous->setNextNode(occNode);
                node= occNode;
                found= true;
            }
            break;
            case Unique:
            {
                node= new UniqueNode();
                previous->setNextNode(node);
                found= true;
            }
            break;
            case Painter:
            {
                PainterNode* painter= new PainterNode();
                m_parsingToolbox->readPainterParameter(painter, str);
                previous->setNextNode(painter);
                node= painter;
                found= true;
            }
            break;
            case ifOperator:
            {
                IfNode* nodeif= new IfNode();
                nodeif->setConditionType(m_parsingToolbox->readConditionType(str));
                Validator* validator= m_parsingToolbox->readCompositeValidator(str);
                if(nullptr != validator)
                {
                    ExecutionNode* trueNode= nullptr;
                    ExecutionNode* falseNode= nullptr;
                    if(readIfInstruction(str, trueNode, falseNode))
                    {
                        nodeif->setInstructionTrue(trueNode);
                        nodeif->setInstructionFalse(falseNode);
                        nodeif->setValidator(validator);
                        previous->setNextNode(nodeif);
                        node= nodeif;
                        found= true;
                    }
                    else
                    {
                        delete nodeif;
                    }
                }
                else
                {
                    delete nodeif;
                }
                break;
            }
            case Split:
            {
                SplitNode* splitnode= new SplitNode();
                previous->setNextNode(splitnode);
                node= splitnode;
                found= true;
            }
            break;
            case Group:
            {
                qint64 groupNumber= 0;
                if(m_parsingToolbox->readNumber(str, groupNumber))
                {
                    GroupNode* groupNode= new GroupNode();
                    groupNode->setGroupValue(groupNumber);
                    previous->setNextNode(groupNode);
                    node= groupNode;
                    found= true;
                }
            }
            break;
            }
        }
    }
    return found;
}
bool DiceParser::readIfInstruction(QString& str, ExecutionNode*& trueNode, ExecutionNode*& falseNode)
{
    if(readBlocInstruction(str, trueNode))
    {
        if(readBlocInstruction(str, falseNode))
        {
            return true;
        }
        return true;
    }
    return false;
}

bool DiceParser::readParameterNode(QString& str, ExecutionNode*& node)
{
    if(str.startsWith("("))
    {
        str= str.remove(0, 1);
        if(readExpression(str, node))
        {
            if(str.startsWith(")"))
            {
                str= str.remove(0, 1);
                return true;
            }
        }
    }
    return false;
}

bool DiceParser::readBlocInstruction(QString& str, ExecutionNode*& resultnode)
{
    if(str.startsWith('{'))
    {
        str= str.remove(0, 1);
        ExecutionNode* node= nullptr;
        Die::ArithmeticOperator op;
        ScalarOperatorNode* scalarNode= nullptr;
        if(m_parsingToolbox->readArithmeticOperator(str, op))
        {
            scalarNode= new ScalarOperatorNode();
            scalarNode->setArithmeticOperator(op);
        }
        if(readExpression(str, node))
        {
            if(str.startsWith('}'))
            {
                if(nullptr == scalarNode)
                {
                    resultnode= node;
                }
                else
                {
                    resultnode= scalarNode;
                    scalarNode->setInternalNode(node);
                }
                str= str.remove(0, 1);
                return true;
            }
        }
    }
    return false;
}

QString DiceParser::getComment() const
{
    return m_comment;
}

void DiceParser::setComment(const QString& comment)
{
    m_comment= comment;
}

QMap<ExecutionNode::DICE_ERROR_CODE, QString> DiceParser::getErrorMap()
{
    QMap<ExecutionNode::DICE_ERROR_CODE, QString> map;

    for(auto start : m_startNodes)
    {
        auto mapTmp= start->getExecutionErrorMap();
        auto keys= mapTmp.keys();
        for(auto& key : keys)
        {
            map.insertMulti(key, mapTmp[key]);
        }
    }
    return map;
}
QString DiceParser::humanReadableError()
{
    QMapIterator<ExecutionNode::DICE_ERROR_CODE, QString> i(m_errorMap);
    QString str("");
    while(i.hasNext())
    {
        i.next();
        str.append(i.value());
        str.append(QStringLiteral("\n"));
    }

    /// list
    QMapIterator<ExecutionNode::DICE_ERROR_CODE, QString> j(getErrorMap());
    while(j.hasNext())
    {
        j.next();
        str.append(j.value());
        str.append(QStringLiteral("\n"));
    }
    return str;
}

QString DiceParser::humanReadableWarning()
{
    QMapIterator<ExecutionNode::DICE_ERROR_CODE, QString> i(m_warningMap);
    QString str("");
    while(i.hasNext())
    {
        i.next();
        str.append(i.value());
        str.append(QStringLiteral("\n"));
    }
    return str;
}

void DiceParser::writeDownDotTree(QString filepath)
{
    if(m_startNodes.empty())
        return;

    QString str(QStringLiteral("digraph ExecutionTree {\n"));
    for(auto start : m_startNodes)
    {
        start->generateDotTree(str);
    }
    str.append(QStringLiteral("}\n"));

    QFile file(filepath);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream in(&file);
        in << str;
    }
}
void DiceParser::setPathToHelp(QString l)
{
    m_helpPath= l;
}
void DiceParser::setVariableDictionary(const QHash<QString, QString>& variables)
{
    ParsingToolBox::setVariableHash(variables);
}
