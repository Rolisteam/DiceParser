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
#ifndef DICEPARSER_H
#define DICEPARSER_H

#include <QMap>
#include <QString>

#include "node/dicerollernode.h"
#include "node/executionnode.h"

#include "booleancondition.h"
#include "dicealias.h"
#include "highlightdice.h"
#include "parsingtoolbox.h"
#include "range.h"
#include "validator.h"

typedef QList<HighLightDice> ListDiceResult;
typedef QMap<quint64, ListDiceResult> ExportedDiceResult;

class ExplodeDiceNode;
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
     * @brief The DiceOperator enum gathering all dice operators
     */
    enum DiceOperator
    {
        D,
        L
    };
    /**
     * @brief The DiceSymbol enum
     */
    enum NodeAction
    {
        JumpBackward
    };
    /**
     * @brief The OptionOperator enum gathering all options  availables for result.
     */
    enum OptionOperator
    {
        KeepAndExplode,
        Keep,
        Reroll,
        RerollUntil,
        Explode,
        Sort,
        Count,
        RerollAndAdd,
        Merge,
        ifOperator,
        Painter,
        Filter,
        Split,
        Group,
        Occurences
        Unique,
        Bind
    };
    /**
     * @brief The CommandOperator enum
     */
    enum CommandOperator
    {
    };

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
     * @brief displayResult
     */
    QString displayResult();
    /**
     * @brief readExpression
     * @param str
     * @param node
     * @return
     */
    bool readExpression(QString& str, ExecutionNode*& node);
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
    QList<DiceAlias*>* getAliases();
    /**
     * @brief insertAlias
     */
    void insertAlias(DiceAlias*, int);
    /**
     * @brief DiceParser::convertAlias
     * @param str
     * @return
     */
    QString convertAlias(QString str);
    /**
     * @brief getErrorList
     * @return
     */
    QMap<ExecutionNode::DICE_ERROR_CODE, QString> getErrorMap();
    /**
     * @brief setPathToHelp set the path to the documentation, this path must be adatped to the lang of application etc…
     * @param l the path.
     */
    void setPathToHelp(QString l);
    /**
     * @brief getAllStringResult
     * @return
     */
    QStringList getAllStringResult(bool& hasAlias);
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
     * @brief readIfInstruction reads the current command to build if node with proper parameters.
     * @param str is the command string, if IF istruction is found, the str will be changed, in other case the string is
     * unmodified
     * @param trueNode is the branch's beginning to be executed if the IfNode is true.
     * @param falseNode is the branch's beginning to be executed if the IfNode is false.
     * @return true, ifNode has been found, false otherwise
     */
    bool readIfInstruction(QString& str, ExecutionNode*& trueNode, ExecutionNode*& falseNode);
    /**
     * @brief setVariableDictionary
     * @param variables
     */
    void setVariableDictionary(const QHash<QString, QString>& variables);
    QString getComment() const;
    void setComment(const QString& comment);

    bool readOptionFromNull(QString& str, ExecutionNode*& node);
    bool readOperatorFromNull(QString& str, ExecutionNode*& node);

    bool readInstructionList(QString& str);
    void getDiceResultFromAllInstruction(QList<ExportedDiceResult>& resultList);
    QString humanReadableWarning();

protected:
    bool readParameterNode(QString& str, ExecutionNode*& node);

private:
    /**
     * @brief readDice
     * @param str
     * @return
     */
    bool readDice(QString& str, ExecutionNode*& node);
    /**
     * @brief readDiceOperator
     * @return
     */
    bool readDiceOperator(QString&, DiceOperator&);
    /**
     * @brief readDiceExpression
     * @param node
     * @return
     */
    bool readDiceExpression(QString&, ExecutionNode*& node);
    /**
     * @brief readOperator
     * @return
     */
    bool readOperator(QString&, ExecutionNode* previous);
    /**
     * @brief DiceParser::readCommand
     * @param str
     * @param node
     * @return
     */
    bool readCommand(QString& str, ExecutionNode*& node);

    /**
     * @brief readOption
     */
    bool readOption(QString&, ExecutionNode* node); // OptionOperator& option,

    /**
     * @brief addRollDiceNode
     * @param faces
     * @return
     */
    DiceRollerNode* addRollDiceNode(qint64 faces, ExecutionNode*);
    /**
     * @brief addExplodeDiceNode
     * @param faces
     * @param previous
     * @return
     */
    ExplodeDiceNode* addExplodeDiceNode(qint64 faces, ExecutionNode* previous);
    /**
     * @brief readOperand
     * @param node
     * @return
     */
    bool readOperand(QString&, ExecutionNode*& node);

    /**
     * @brief readInstructionOperator
     * @param c
     * @return
     */
    bool readInstructionOperator(QChar c);
    /**
     * @brief readNode
     * @param str
     * @param node
     * @return
     */
    bool readNode(QString& str, ExecutionNode*& node);

    /**
     * @brief getLeafNode
     * @return
     */
    ExecutionNode* getLeafNode(ExecutionNode* node);

    /**
     * @brief hasResultOfType
     * @param notthelast
     * @return
     */
    bool hasResultOfType(Result::RESULT_TYPE, ExecutionNode* node, bool notthelast= false);

private:
    QMap<QString, DiceOperator>* m_mapDiceOp;
    QMap<QString, OptionOperator>* m_OptionOp;
    QMap<QString, NodeAction>* m_nodeActionMap;
    QList<DiceAlias*>* m_aliasList;
    QStringList* m_commandList;

    QMap<ExecutionNode::DICE_ERROR_CODE, QString> m_errorMap;
    QMap<ExecutionNode::DICE_ERROR_CODE, QString> m_warningMap;

    ExecutionNode* m_start= nullptr;
    std::vector<ExecutionNode*> m_startNodes;
    // ExecutionNode* m_current;
    QString m_command;
    ParsingToolBox* m_parsingToolbox;
    QString m_helpPath;
    bool m_currentTreeHasSeparator;
    bool readBlocInstruction(QString& str, ExecutionNode*& resultnode);
    QString m_comment;
};

#endif // DICEPARSER_H
