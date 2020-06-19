/***************************************************************************
 * Copyright (C) 2014 by Renaud Guezennec                                   *
 * https://rolisteam.org/contact                      *
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

const QList<DiceAlias*>& DiceParser::constAliases() const
{
    return m_parsingToolbox->getAliases();
}

QList<DiceAlias*>* DiceParser::aliases() const
{
    return m_parsingToolbox->aliases();
}

void DiceParser::cleanAliases()
{
    m_parsingToolbox->cleanUpAliases();
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
    auto instructions= m_parsingToolbox->readInstructionList(str, true);
    m_command.remove(m_parsingToolbox->getComment());
    bool value= !instructions.empty();
    if(!value)
    {
        m_parsingToolbox->addError(
            Dice::ERROR_CODE::NOTHING_UNDERSTOOD,
            QObject::tr("Nothing was understood. To roll dice: !1d6 - full documation: "
                        "<a "
                        "href=\"https://github.com/Rolisteam/DiceParser/blob/master/HelpMe.md\">https://github.com/"
                        "Rolisteam/DiceParser/blob/master/HelpMe.md</a>"));
    }
    else if(value && !str.isEmpty())
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

QString DiceParser::convertAlias(const QString& cmd) const
{
    return m_parsingToolbox->convertAlias(cmd);
}

void DiceParser::start()
{
    for(auto start : m_parsingToolbox->getStartNodes())
    {
        start->run();
    }
}

/*QList<qreal> DiceParser::lastIntegerResults() const
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
QStringList DiceParser::stringResult() const
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
QStringList DiceParser::allFirstResultAsString(bool& hasAlias)
{
    // QStringList allResult;
    QStringList stringListResult;
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        QVariant var;
        if(hasResultOfType(Dice::RESULT_TYPE::STRING, node, var))
        {
            stringListResult << var.toString();
            hasAlias= true;
        }
        else if(hasResultOfType(Dice::RESULT_TYPE::SCALAR, node, var, true))
        {
            stringListResult << QString::number(var.toReal());
            hasAlias= true;
        }
    }
    return stringListResult;
}
QStringList DiceParser::allDiceResult(bool& hasAlias) const
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
}*/

void DiceParser::diceResultFromEachInstruction(QList<ExportedDiceResult>& resultList) const
{
    for(auto start : m_parsingToolbox->getStartNodes())
    {
        ExecutionNode* next= ParsingToolBox::getLeafNode(start);
        Result* result= next->getResult();
        ExportedDiceResult nodeResult;
        QSet<QString> alreadyAdded;
        // qDebug() << "start";
        while(nullptr != result)
        {
            if(result->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST))
            {
                DiceResult* diceResult= dynamic_cast<DiceResult*>(result);
                QList<HighLightDice> list;
                quint64 faces= 0;
                // qDebug() << "Begin of loop";
                for(auto& die : diceResult->getResultList())
                {
                    faces= die->getFaces();
                    // qDebug() << "face" << faces << die->getValue() <<
                    // die->getListValue()
                    //         << next->toString(true);
                    HighLightDice hlDice(die->getListValue(), die->isHighlighted(), die->getColor(),
                                         die->hasBeenDisplayed(), die->getFaces());
                    // qDebug() << die->getListValue() << die->getFaces() << die->getUuid() << list.size();
                    if(!alreadyAdded.contains(die->getUuid()))
                    {
                        list.append(hlDice);
                        alreadyAdded.insert(die->getUuid());
                    }
                }
                // qDebug() << "End of loop" << list.size();
                if(!list.isEmpty())
                    nodeResult.insert(faces, list);
            }
            if(nodeResult.isEmpty())
                result= result->getPrevious();
            else
                result= nullptr;
        }
        // qDebug() << "end";
        resultList.append(nodeResult);
    }
    // qDebug() << resultList.size();
}

/*void DiceParser::lastDiceResult(QList<ExportedDiceResult>& diceValuesList, bool& homogeneous) const
{
    QSet<QString> alreadySeenDice;
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
}*/

QString DiceParser::diceCommand() const
{
    return m_command;
}

bool DiceParser::hasIntegerResultNotInFirst() const
{
    bool result= false;
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        QVariant var;
        result|= hasResultOfType(Dice::RESULT_TYPE::SCALAR, node, var, true);
    }
    return result;
}

bool DiceParser::hasDiceResult() const
{
    bool result= false;
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        QVariant var;
        result|= hasResultOfType(Dice::RESULT_TYPE::DICE_LIST, node, var);
    }
    return result;
}
bool DiceParser::hasStringResult() const
{
    bool result= false;
    for(auto node : m_parsingToolbox->getStartNodes())
    {
        QVariant var;
        result|= hasResultOfType(Dice::RESULT_TYPE::STRING, node, var);
    }
    return result;
}
bool DiceParser::hasResultOfType(Dice::RESULT_TYPE type, ExecutionNode* node, QVariant& value, bool notthelast)
{
    bool scalarDone= false;
    ExecutionNode* next= ParsingToolBox::getLeafNode(node);
    Result* result= next->getResult();
    while((result != nullptr) && (!scalarDone))
    {
        bool lastResult= false;
        if(notthelast)
            lastResult= (nullptr == result->getPrevious());

        if(result->hasResultOfType(type) && !lastResult)
        {
            scalarDone= true;
            value= result->getResult(type);
        }
        result= result->getPrevious();
    }
    return scalarDone;
}

/*QList<qreal> DiceParser::sumOfDiceResult() const
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
}*/

int DiceParser::startNodeCount() const
{
    return static_cast<int>(m_parsingToolbox->getStartNodes().size());
}

QString DiceParser::comment() const
{
    return m_parsingToolbox->getComment();
}

void DiceParser::setComment(const QString& comment)
{
    m_parsingToolbox->setComment(comment);
}

QMap<Dice::ERROR_CODE, QString> DiceParser::errorMap() const
{
    QMap<Dice::ERROR_CODE, QString> map;

    for(auto start : m_parsingToolbox->getStartNodes())
    {
        auto mapTmp= start->getExecutionErrorMap();
        auto keys= mapTmp.keys();
        for(auto& key : keys)
        {
            map.insert(key, mapTmp[key]);
        }
    }
    return map;
}
QString DiceParser::humanReadableError() const
{
    auto parsingError= m_parsingToolbox->getErrorList();
    QString str;
    std::for_each(parsingError.begin(), parsingError.end(), [&str](const QString& text) {
        str.append(text);
        str.append(QStringLiteral("\n"));
    });

    /// list
    auto errMap= errorMap();
    std::for_each(errMap.begin(), errMap.end(), [&str](const QString& text) {
        str.append(text);
        str.append(QStringLiteral("\n"));
    });
    return str;
}

QString DiceParser::humanReadableWarning() const
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

QJsonObject DiceParser::exportResult() const
{
    QJsonObject obj;
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
