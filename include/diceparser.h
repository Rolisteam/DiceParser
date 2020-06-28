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

    /**
     * @brief parseLine, method to call for starting the dice roll. It will parse the command and run the execution
     * tree.
     * @param str dice command
     * @return bool every thing is fine or not
     */
    bool parseLine(QString str, bool allowAlias= true);
    QString convertAlias(const QString& cmd) const;
    /**
     * @brief getStartNodeCount
     * @return
     */
    int getStartNodeCount() const;
    /**
     * @brief Start running the execution tree
     *
     */
    void start();
    /**
     * @brief displayDotTree - Write the execution tree into file using dot format.
     * @param filepath absolute or relative path to the tree file.
     */
    void writeDownDotTree(QString filepath);
    /**
     * @brief getLastIntegerResults
     * @return
     */
    QList<qreal> getLastIntegerResults();
    /**
     * @brief getSumOfDiceResult
     * @return
     */
    QList<qreal> getSumOfDiceResult();
    /**
     * @brief getLastDiceResult
     * @return
     */
    void getLastDiceResult(QList<ExportedDiceResult>& diceValues, bool& homogeneous);
    /**
     * @brief hasIntegerResultNotInFirst
     * @return
     */
    bool hasIntegerResultNotInFirst();
    /**
     * @brief hasDiceResult
     * @return
     */
    bool hasDiceResult();
    /**
     * @brief getDiceCommand
     * @return
     */
    QString getDiceCommand() const;
    /**
     * @brief hasStringResult
     * @return
     */
    bool hasStringResult();
    /**
     * @brief getStringResult
     * @return
     */
    QStringList getStringResult();
    /**
     * @brief humanReadableError
     * @return
     */
    QString humanReadableError();
    /**
     * @brief getAliases
     * @return
     */
    const QList<DiceAlias*>& getAliases() const;
    QList<DiceAlias*>* aliases() const;
    void cleanAliases();
    /**
     * @brief insertAlias
     */
    void insertAlias(DiceAlias*, int);
    /**
     * @brief getErrorList
     * @return
     */
    QMap<Dice::ERROR_CODE, QString> errorMap() const;
    /**
     * @brief setPathToHelp set the path to the documentation, this path must be adatped to the lang of application etc…
     * @param l the path.
     */
    void setPathToHelp(QString l);
    /**
     * @brief allFirstResultAsString
     * @return
     */
    QStringList allFirstResultAsString(bool& hasAlias);
    /**
     * @brief getAllDiceResult
     * @param hasAlias
     * @return
     */
    QStringList getAllDiceResult(bool& hasAlias);
    /**
     * @brief hasSeparator allows to know if the current command has separator.
     * @return true when the command has separator, false otherwise.
     */
    bool hasSeparator() const;

    /**
     * @brief setVariableDictionary
     * @param variables
     */
    void setVariableDictionary(const QHash<QString, QString>& variables);
    QString getComment() const;
    void setComment(const QString& comment);

    void getDiceResultFromAllInstruction(QList<ExportedDiceResult>& resultList);
    QString humanReadableWarning();

    void cleanAll();

private:
    /**
     * @brief hasResultOfType
     * @param notthelast
     * @return
     */
    bool hasResultOfType(Dice::RESULT_TYPE, ExecutionNode* node, QVariant& value, bool notthelast= false);
    bool readBlocInstruction(QString& str, ExecutionNode*& resultnode);

private:
    std::unique_ptr<ParsingToolBox> m_parsingToolbox;
    QString m_command;
};

#endif // DICEPARSER_H
