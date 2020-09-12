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
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QStringList>
#include <functional>
#include <numeric>

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
        m_parsingToolbox->addError(Dice::ERROR_CODE::NOTHING_UNDERSTOOD,
                                   QObject::tr("Nothing was understood. To roll dice: !1d6 - full documation: "
                                               "<a "
                                               "href=\"https://github.com/Rolisteam/DiceParser/blob/master/"
                                               "HelpMe.md\">https://github.com/"
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

/*
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

/*void DiceParser::lastDiceResult(QList<ExportedDiceResult>& diceValuesList,
bool& homogeneous) const
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
                        if(die->hasBeenDisplayed() ||
alreadySeenDice.contains(die->getUuid())) continue;

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
                        HighLightDice hlDice(valuesResult, die->isHighlighted(),
die->getColor(), die->hasBeenDisplayed(), 0); listpair.append(hlDice);
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
    return m_parsingToolbox->hasIntegerResultNotInFirst();
}

bool DiceParser::hasDiceResult() const
{
    return m_parsingToolbox->hasDiceResult();
}
bool DiceParser::hasStringResult() const
{
    return m_parsingToolbox->hasStringResult();
}

int DiceParser::startNodeCount() const
{
    return static_cast<int>(m_parsingToolbox->getStartNodes().size());
}

QList<qreal> DiceParser::scalarResultsFromEachInstruction() const
{
    return m_parsingToolbox->scalarResultsFromEachInstruction();
}

QStringList DiceParser::stringResultFromEachInstruction(bool& hasAlias) const
{
    return m_parsingToolbox->allFirstResultAsString(hasAlias);
}

void DiceParser::diceResultFromEachInstruction(QList<ExportedDiceResult>& resultList) const
{
    resultList= m_parsingToolbox->diceResultFromEachInstruction();
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

QString DiceParser::finalStringResult() const
{
    return m_parsingToolbox->finalStringResult();
}

QString DiceParser::resultAsJSon(std::function<QString(const QString&, const QString&, bool)> colorize) const
{
    QJsonObject obj;
    QJsonArray instructions;
    for(auto start : m_parsingToolbox->getStartNodes())
    {
        QJsonObject inst;

        m_parsingToolbox->addResultInJson(inst, Dice::RESULT_TYPE::SCALAR, "scalar", start, true);
        m_parsingToolbox->addResultInJson(inst, Dice::RESULT_TYPE::STRING, "string", start, false);
        m_parsingToolbox->addDiceResultInJson(inst, start, colorize);

        instructions.append(inst);
    }
    obj["instructions"]= instructions;
    obj["comment"]= m_parsingToolbox->getComment();
    obj["error"]= humanReadableError();
    obj["scalar"]= m_parsingToolbox->finalScalarResult().first;
    obj["string"]= m_parsingToolbox->finalStringResult();
    obj["warning"]= humanReadableWarning();
    obj["command"]= m_command;

    QJsonDocument doc;
    doc.setObject(obj);
    return doc.toJson();
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
