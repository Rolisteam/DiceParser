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
#ifndef PARSINGTOOLBOX_H
#define PARSINGTOOLBOX_H

#include <QMap>
#include <vector>

#include "booleancondition.h"
#include "highlightdice.h"
#include "node/dicerollernode.h"
#include "node/executionnode.h"
#include "node/ifnode.h"
#include "node/paintnode.h"
#include "node/scalaroperatornode.h"
#include "operationcondition.h"
#include "range.h"
#include "validatorlist.h"

class RepeaterNode;
class DiceAlias;
class ExplodeDiceNode;

class SubtituteInfo
{
public:
    SubtituteInfo();

    bool isValid() const;

    int length() const;
    void setLength(int length);

    int resultIndex() const;
    void setResultIndex(int valueIndex);

    int position() const;
    void setPosition(int position);

    int digitNumber() const;
    void setDigitNumber(int digitNumber);

private:
    int m_length= 2;
    int m_digitNumber= 0;
    int m_resultIndex= -1;
    int m_position= -1;
};

/**
 * @brief The ParsingToolBox is gathering many useful methods for dice parsing.
 * Its goal is to make the diceparser a bit lighter.
 */
class ParsingToolBox
{
public:
    enum LIST_OPERATOR
    {
        NONE,
        UNIQUE
    };
    enum Function
    {
        REPEAT
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
        Occurences,
        Unique,
        Bind,
        AllSameExplode
    };
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
     * @brief ParsingToolBox
     */
    ParsingToolBox();
    /**
     * @brief ParsingToolBox
     * @param data
     */
    ParsingToolBox(const ParsingToolBox& data);
    /**
     * @brief ~ParsingToolBox
     */
    virtual ~ParsingToolBox();

    void clearUp();
    /**
     * @brief addSort
     * @param e
     * @param b
     * @return
     */
    ExecutionNode* addSort(ExecutionNode* e, bool b);
    /**
     * @brief readAscending
     * @param str
     * @return
     */
    static bool readAscending(QString& str);
    /**
     * @brief readLogicOperator
     * @param str
     * @param op
     * @return
     */
    bool readLogicOperator(QString& str, BooleanCondition::LogicOperator& op);
    /**
     * @brief readValidator
     * @param str
     * @return
     */
    Validator* readValidator(QString& str, bool hasSquare= false);
    /**
     * @brief readCompositeValidator
     * @param str
     * @return
     */
    ValidatorList* readValidatorList(QString& str);

    /**
     * @brief readNumber read number in the given str and remove from the string the read character.
     * @param str the command line
     * @param myNumber reference to the found number
     * @return true, succeed to read number, false otherwise.
     */
    static bool readNumber(QString& str, qint64& myNumber);

    /**
     * @brief readString
     * @param str
     * @param strResult
     * @return
     */
    static bool readString(QString& str, QString& strresult);
    /**
     * @brief readVariable
     * @param str
     * @param myNumber
     * @return
     */
    static bool readVariable(QString& str, qint64& myNumber, QString& reasonFail);
    /**
     * @brief readOpenParentheses
     * @param str
     * @return
     */
    static bool readOpenParentheses(QString& str);
    /**
     * @brief readCloseParentheses
     * @param str
     * @return
     */
    static bool readCloseParentheses(QString& str);

    /**
     * @brief readNumber read number in the given str and remove from the string the read character.
     * @param str the command line
     * @param myNumber reference to the found number
     * @return true, succeed to read number, false otherwise.
     */
    static bool readDynamicVariable(QString& str, qint64& index);

    /**
     * @brief readList
     * @param str
     * @param list
     * @return
     */
    bool readList(QString& str, QStringList& list, QList<Range>& ranges);
    /**
     * @brief isValidValidator
     * @param previous
     * @param val
     * @return
     */
    Dice::CONDITION_STATE isValidValidator(ExecutionNode* previous, ValidatorList* val);
    /**
     * @brief getDiceRollerNode
     * @param previous
     * @return
     */
    DiceRollerNode* getDiceRollerNode(ExecutionNode* previous);

    /**
     * @brief readDiceRange
     * @param str
     * @param start
     * @param end
     * @return
     */
    bool readDiceRange(QString& str, qint64& start, qint64& end);
    /**
     * @brief readListOperator
     * @param str
     * @return
     */
    static LIST_OPERATOR readListOperator(QString& str);

    void readProbability(QStringList& str, QList<Range>& ranges);

    bool readLogicOperation(QString& str, ValidatorList::LogicOperation& op);

    bool readDiceLogicOperator(QString& str, OperationCondition::ConditionOperator& op);

    bool readArithmeticOperator(QString& str, Die::ArithmeticOperator& op);
    std::vector<ExecutionNode*> readInstructionList(QString& str, bool startNode);

    static bool readPainterParameter(PainterNode* painter, QString& str);

    static QHash<QString, QString> getVariableHash();
    static void setVariableHash(const QHash<QString, QString>& variableHash);
    /**
     * @brief readConditionType
     * @param str
     * @return
     */
    static Dice::ConditionType readConditionType(QString& str);

    bool readComment(QString& str, QString&, QString&);
    static ExecutionNode* getLatestNode(ExecutionNode* node);

    const std::vector<ExecutionNode*>& getStartNodes();
    static void setStartNodes(std::vector<ExecutionNode*>* startNodes);

    bool readOperand(QString& str, ExecutionNode*& node);
    static int findClosingCharacterIndexOf(QChar open, QChar closing, const QString& str, int offset);

    static QString replaceVariableToValue(const QString& source, QStringList values);
    static QString replacePlaceHolderToValue(const QString& source, const QList<ExportedDiceResult>& list);

    static SubtituteInfo readVariableFromString(const QString& source, int& start);
    static SubtituteInfo readPlaceHolderFromString(const QString& source, int& start);

    static void readSubtitutionParameters(SubtituteInfo& info, QString& rest);

    static bool readComma(QString& str);

    bool readReaperArguments(RepeaterNode* node, QString& source);

    DiceRollerNode* addRollDiceNode(qint64 faces, ExecutionNode*);

    ExplodeDiceNode* addExplodeDiceNode(qint64 faces, ExecutionNode* previous);

    bool readExpression(QString& str, ExecutionNode*& node);

    static ExecutionNode* getLeafNode(ExecutionNode* start);

    bool hasError() const;

    bool readInstructionOperator(QChar c);

    bool readNode(QString& str, ExecutionNode*& node);
    /**
     * @brief readIfInstruction reads the current command to build if node with proper parameters.
     * @param str is the command string, if IF istruction is found, the str will be changed, in other case the string is
     * unmodified
     * @param trueNode is the branch's beginning to be executed if the IfNode is true.
     * @param falseNode is the branch's beginning to be executed if the IfNode is false.
     * @return true, ifNode has been found, false otherwise
     */
    bool readIfInstruction(QString& str, ExecutionNode*& trueNode, ExecutionNode*& falseNode);

    bool readOptionFromNull(QString& str, ExecutionNode*& node);
    bool readOperatorFromNull(QString& str, ExecutionNode*& node);
    bool readParameterNode(QString& str, ExecutionNode*& node);

    /**
     *
     */
    bool readFunction(QString& str, ExecutionNode*& node);
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

    bool readValuesList(QString& str, ExecutionNode*& node);

    void addError(Dice::ERROR_CODE code, const QString& msg);
    void addWarning(Dice::ERROR_CODE code, const QString& msg);

    void setComment(const QString& comment);
    QString getComment() const;

    const QMap<Dice::ERROR_CODE, QString>& getErrorList() const;
    const QMap<Dice::ERROR_CODE, QString>& getWarningList() const;

    QString convertAlias(QString str);

    bool readBlocInstruction(QString& str, ExecutionNode*& resultnode);

    void setHelpPath(const QString& path);

    void insertAlias(DiceAlias* dice, int i);
    const QList<DiceAlias*>& getAliases() const;
    QList<DiceAlias*>* aliases();
    void cleanUpAliases();

private:
    QMap<QString, BooleanCondition::LogicOperator> m_logicOp;
    QMap<QString, ValidatorList::LogicOperation> m_logicOperation;
    QMap<QString, OperationCondition::ConditionOperator> m_conditionOperation;
    QHash<QString, Die::ArithmeticOperator> m_arithmeticOperation;
    QMap<QString, DiceOperator> m_mapDiceOp;
    QMap<QString, OptionOperator> m_OptionOp;
    QMap<QString, NodeAction> m_nodeActionMap;
    std::map<QString, Function> m_functionMap;
    QStringList m_commandList;

    QMap<Dice::ERROR_CODE, QString> m_errorMap;
    QMap<Dice::ERROR_CODE, QString> m_warningMap;
    std::vector<ExecutionNode*> m_startNodes;

    QString m_comment;

    static QHash<QString, QString> m_variableHash;
    QString m_helpPath;
    QList<DiceAlias*> m_aliasList;
};

#endif // PARSINGTOOLBOX_H
