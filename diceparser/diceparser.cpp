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

#include "booleancondition.h"
#include "dicealias.h"
#include "parsingtoolbox.h"
#include "range.h"
#include "result/stringresult.h"
#include "validator.h"

#define DEFAULT_FACES_NUMBER 10

DiceParser::DiceParser() : m_parsingToolbox(new ParsingToolBox()) {}
DiceParser::~DiceParser() {}

const QList<DiceAlias*>& DiceParser::getAliases() const
{
    return m_parsingToolbox->getAliases();
}
void DiceParser::insertAlias(DiceAlias* dice, int i)
{
    m_parsingToolbox->insertAlias(dice, i);
}

bool DiceParser::parseLine(QString str, bool allowAlias)
{
    if(allowAlias)
    {
        str= m_parsingToolbox->convertAlias(str);
    }
    m_command= str;
    bool hasInstruction= m_parsingToolbox->readInstructionList(str);
    m_command.remove(m_parsingToolbox->getComment());
    bool value= hasInstruction;
    if(!hasInstruction)
    {
        m_parsingToolbox->addError(
            Dice::ERROR_CODE::NOTHING_UNDERSTOOD,
            QObject::tr("Nothing was understood. To roll dice: !1d6 - full documation: "
                        "<a "
                        "href=\"https://github.com/Rolisteam/DiceParser/blob/master/HelpMe.md\">https://github.com/"
                        "Rolisteam/DiceParser/blob/master/HelpMe.md</a>"));
    }
    else if(hasInstruction && !str.isEmpty())
    {
        auto i= m_command.size() - str.size();
        m_parsingToolbox->addWarning(
            Dice::ERROR_CODE::UNEXPECTED_CHARACTER,
            QObject::tr("Unexpected character at %1 - end of command was ignored \"%2\"").arg(i).arg(str));
    }

    if(m_parsingToolbox->hasError())
        value= false;

    return value;
}

void DiceParser::start()
{
    for(auto start : m_parsingToolbox->getStartNodes())
    {
        start->run();
    }
}

QList<qreal> DiceParser::getLastIntegerResults()
{
    QList<qreal> resultValues;
    QStringList alreadyVisitedNode;
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        ExecutionNode* next= ParsingToolBox::getLeafNode(node);
        Result* result= next->getResult();
        bool scalarDone= false;
        while((result != nullptr) && (!scalarDone))
        {
            if(result->hasResultOfType(Dice::RESULT_TYPE::SCALAR))
            {
                if(!alreadyVisitedNode.contains(result->getId()))
                {
                    resultValues << result->getResult(Dice::RESULT_TYPE::SCALAR).toReal();
                    alreadyVisitedNode << result->getId();
                }
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
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        ExecutionNode* next= ParsingToolBox::getLeafNode(node);
        QString str;
        Result* result= next->getResult();
        bool found= false;
        while((nullptr != result) && (!found))
        {
            if(result->hasResultOfType(Dice::RESULT_TYPE::STRING))
            {
                str= result->getResult(Dice::RESULT_TYPE::STRING).toString();
                found= true;
            }
            result= result->getPrevious();
        }
        if(!str.isEmpty())
            stringListResult << str;
    }
    return stringListResult;
}
QStringList DiceParser::getAllStringResult(bool& hasAlias)
{
    // QStringList allResult;
    QStringList stringListResult;
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        ExecutionNode* next= ParsingToolBox::getLeafNode(node);
        Result* result= next->getResult();

        while(nullptr != result)
        {
            if(result->hasResultOfType(Dice::RESULT_TYPE::STRING))
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
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        ExecutionNode* next= ParsingToolBox::getLeafNode(node);
        Result* result= next->getResult();
        QList<Die*> dieListResult;

        while(nullptr != result)
        {
            if(result->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST))
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
    for(auto start : m_parsingToolbox->getStartNodes())
    {
        ExecutionNode* next= ParsingToolBox::getLeafNode(start);
        Result* result= next->getResult();
        ExportedDiceResult nodeResult;
        while(nullptr != result)
        {
            if(result->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST))
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
    QStringList alreadySeenDice;
    for(auto start : m_parsingToolbox->getStartNodes())
    {
        ExportedDiceResult diceValues;
        ExecutionNode* next= ParsingToolBox::getLeafNode(start);
        Result* result= next->getResult();
        while(nullptr != result)
        {
            if(result->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST))
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
                        if(die->hasBeenDisplayed() || alreadySeenDice.contains(die->getUuid()))
                            continue;

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
                        alreadySeenDice << die->getUuid();
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
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        result|= hasResultOfType(Dice::RESULT_TYPE::SCALAR, node);
    }
    return result;
}

bool DiceParser::hasDiceResult()
{
    bool result= false;
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        result|= hasResultOfType(Dice::RESULT_TYPE::DICE_LIST, node);
    }
    return result;
}
bool DiceParser::hasStringResult()
{
    bool result= false;
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        result|= hasResultOfType(Dice::RESULT_TYPE::STRING, node);
    }
    return result;
}
bool DiceParser::hasResultOfType(Dice::RESULT_TYPE type, ExecutionNode* node, bool notthelast)
{
    bool scalarDone= false;
    ExecutionNode* next= ParsingToolBox::getLeafNode(node);
    Result* result= next->getResult();
    while((result != nullptr) && (!scalarDone))
    {
        bool lastResult= false;
        if(notthelast)
            lastResult= (nullptr != result->getPrevious());

        if(result->hasResultOfType(type) && !lastResult)
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
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        qreal resultValue= 0;
        ExecutionNode* next= ParsingToolBox::getLeafNode(node);
        Result* result= next->getResult();
        bool found= false;
        while((nullptr != result) && (!found))
        {
            if(result->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST))
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
    return static_cast<int>(m_parsingToolbox->getStartNodes().size());
}

QString DiceParser::getComment() const
{
    return m_parsingToolbox->getComment();
}

void DiceParser::setComment(const QString& comment)
{
    m_parsingToolbox->setComment(comment);
}

QMap<Dice::ERROR_CODE, QString> DiceParser::getErrorMap()
{
    QMap<Dice::ERROR_CODE, QString> map;

    for(auto start : m_parsingToolbox->getStartNodes())
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
    auto errorMap= m_parsingToolbox->getErrorList();
    QMapIterator<Dice::ERROR_CODE, QString> i(errorMap);
    QString str("");
    while(i.hasNext())
    {
        i.next();
        str.append(i.value());
        str.append(QStringLiteral("\n"));
    }

    /// list
    QMapIterator<Dice::ERROR_CODE, QString> j(getErrorMap());
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
    auto warningMap= m_parsingToolbox->getWarningList();
    QMapIterator<Dice::ERROR_CODE, QString> i(warningMap);
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
    if(m_parsingToolbox->getStartNodes().empty())
        return;

    QString str(QStringLiteral("digraph ExecutionTree {\n"));
    for(auto start : m_parsingToolbox->getStartNodes())
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
    m_parsingToolbox->setHelpPath(l);
}
void DiceParser::setVariableDictionary(const QHash<QString, QString>& variables)
{
    ParsingToolBox::setVariableHash(variables);
}
