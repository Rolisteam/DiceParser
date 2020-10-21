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
#ifndef DICEPARSER_H
#define DICEPARSER_H

#include <QJsonObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include <memory>
#include <vector>

#include "diceparserhelper.h"
#include "highlightdice.h"
//#include "node/executionnode.h"

class ExplodeDiceNode;
class ParsingToolBox;
class DiceRollerNode;
class DiceAlias;
class ExecutionNode;
/**
 * @page DiceParser Dice Parser
 *
 * @section Intro Introduction
 * Diceparser is the software component dedicated to compute dice command in rolisteam.<br/>
 *
 * @section grammar The Grammar
 *
 * The grammar is described in Readme.md
 */

/**
 * @brief The DiceParser class facade class, it receives a command and return a DiceResult class (not yet implemented).
 */
class DiceParser
{
public:
    /**
     * @brief DiceParser default constructor
     */
    DiceParser();
    /**
     * @brief ~DiceParser
     */
    virtual ~DiceParser();

    // Command process methods
    /**
     * @brief parseLine, method to call for starting the dice roll. It will parse the command and run the execution
     * tree.
     * @param str dice command
     * @return bool every thing is fine or not
     */
    bool parseLine(QString str, bool allowAlias= true);
    void start();
    void cleanAll();

    // debug
    void writeDownDotTree(QString filepath);

    // control methods
    bool hasIntegerResultNotInFirst() const;
    bool hasDiceResult() const;
    bool hasStringResult() const;
    bool hasSeparator() const;

    // alias management
    const QList<DiceAlias*>& constAliases() const;
    QList<DiceAlias*>* aliases() const;
    void cleanAliases();
    void insertAlias(DiceAlias*, int);
    QString convertAlias(const QString& cmd) const;

    QStringList allFirstResultAsString(bool& hasAlias);
    QStringList getAllDiceResult(bool& hasAlias);

    // Accessors
    int startNodeCount() const;
    QList<qreal> scalarResultsFromEachInstruction() const;
    QStringList stringResultFromEachInstruction(bool& hasAlias) const;
    void diceResultFromEachInstruction(QList<ExportedDiceResult>& resultList) const;
    QString finalStringResult(std::function<QString(const QString&, const QString&, bool)> colorize) const;

    QString diceCommand() const;
    QMap<Dice::ERROR_CODE, QString> errorMap() const;
    QString comment() const;
    QString humanReadableWarning() const;
    QString humanReadableError() const;
    QString resultAsJSon(std::function<QString(const QString&, const QString&, bool)> colorize,
                         bool removeUnhighligthed= false) const;

    //    QStringList stringResult() const;
    //    QStringList allDiceResult(bool& hasAlias) const;
    //    void lastDiceResult(QList<ExportedDiceResult>& diceValues, bool& homogeneous) const;

    // setters
    void setPathToHelp(QString l);
    void setVariableDictionary(const QHash<QString, QString>& variables);
    void setComment(const QString& comment);

private:
    bool readBlocInstruction(QString& str, ExecutionNode*& resultnode);

private:
    std::unique_ptr<ParsingToolBox> m_parsingToolbox;
    QString m_command;
};

#endif // DICEPARSER_H
