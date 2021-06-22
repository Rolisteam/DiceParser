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
#include <functional>
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
class SwitchCaseNode;
class ReplaceValueNode;

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

    int subIndex() const;
    void setSubIndex(int subindex);

private:
    int m_length= 2;
    int m_digitNumber= 0;
    int m_resultIndex= -1;
    int m_position= -1;
    int m_subIndex= -1;
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
        NONE= 0x00,
        UNIQUE= 0x01,
        NOCOMMA= 0x02,
        UniqueAndNoComma= 0x03
    };

    enum Function
    {
        REPEAT
    };
    enum OptionOperator
    {
        KeepAndExplode,   // K
        Keep,             // k
        Reroll,           // r
        RerollUntil,      // R
        Explode,          // e
        Sort,             // s
        Count,            // c
        RerollAndAdd,     // a
        Merge,            // m
        ifOperator,       // i
        Painter,          // p
        Filter,           // f
        Split,            // y
        Group,            // g
        Occurences,       // o
        Unique,           // u
        Bind,             // b
        AllSameExplode,   // t
        SwitchCaseOption, // S
        TransformOption   // T
    };
    enum DiceOperator
    {
        D,
        L
    };
    enum NodeAction
    {
        JumpBackward
    };
    ParsingToolBox();
    ParsingToolBox(const ParsingToolBox& data);
    virtual ~ParsingToolBox();
    void clearUp();

    // Build execution tree
    DiceRollerNode* getDiceRollerNode(ExecutionNode* previous);
    DiceRollerNode* addRollDiceNode(qint64 faces, ExecutionNode*);
    ExplodeDiceNode* addExplodeDiceNode(qint64 faces, ExecutionNode* previous);
    Dice::CONDITION_STATE isValidValidator(ExecutionNode* previous, ValidatorList* val);
    ExecutionNode* addSort(ExecutionNode* e, bool b);

    // parsing tools
    static bool readAscending(QString& str);
    static bool readStopAtFirst(QString& str);
    bool readLogicOperator(QString& str, BooleanCondition::LogicOperator& op);
    Validator* readValidator(QString& str, bool hasSquare= false);
    ValidatorList* readValidatorList(QString& str);
    static bool readNumber(QString& str, qint64& myNumber);
    static bool readString(QString& str, QString& strresult);
    static bool readVariable(QString& str, qint64& myNumber, QString& reasonFail);
    static bool readOpenParentheses(QString& str);
    static bool readCloseParentheses(QString& str);

    static bool readDynamicVariable(QString& str, qint64& index);
    bool readList(QString& str, QStringList& list, QList<Range>& ranges);
    bool readDiceRange(QString& str, qint64& start, qint64& end);
    static LIST_OPERATOR readListOperator(QString& str);
    void readProbability(QStringList& str, QList<Range>& ranges);
    bool readLogicOperation(QString& str, ValidatorList::LogicOperation& op);
    bool readDiceLogicOperator(QString& str, OperationCondition::ConditionOperator& op);
    bool readArithmeticOperator(QString& str, Die::ArithmeticOperator& op);
    std::vector<ExecutionNode*> readInstructionList(QString& str, bool startNode);
    static Dice::ConditionType readConditionType(QString& str);
    bool readComment(QString& str, QString&, QString&);
    bool readOperand(QString& str, ExecutionNode*& node);
    static int findClosingCharacterIndexOf(QChar open, QChar closing, const QString& str, int offset);
    static void readSubtitutionParameters(SubtituteInfo& info, QString& rest);
    static bool readPainterParameter(PainterNode* painter, QString& str);
    static bool readComma(QString& str);
    bool readReaperArguments(RepeaterNode* node, QString& source);
    bool readExpression(QString& str, ExecutionNode*& node);
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
    bool readFunction(QString& str, ExecutionNode*& node);
    bool readDice(QString& str, ExecutionNode*& node);
    bool readDiceOperator(QString&, DiceOperator&);
    bool readDiceExpression(QString&, ExecutionNode*& node);
    bool readOperator(QString&, ExecutionNode* previous);
    bool readCommand(QString& str, ExecutionNode*& node);
    bool readBlocInstruction(QString& str, ExecutionNode*& resultnode);
    bool readOption(QString&, ExecutionNode* node); // OptionOperator& option,
    bool readValuesList(QString& str, ExecutionNode*& node);
    bool readSwitchCaseNode(QString& str, SwitchCaseNode* node);
    bool readReplaceValueNode(QString& str, ReplaceValueNode* node);

    // Error
    bool hasError() const;
    void addError(Dice::ERROR_CODE code, const QString& msg);
    void addWarning(Dice::ERROR_CODE code, const QString& msg);
    const QMap<Dice::ERROR_CODE, QString>& getErrorList() const;
    const QMap<Dice::ERROR_CODE, QString>& getWarningList() const;

    // Traversal functions
    static QString number(qreal value);
    static ExecutionNode* getLeafNode(ExecutionNode* start);
    const std::vector<ExecutionNode*>& getStartNodes();
    static void setStartNodes(std::vector<ExecutionNode*>* startNodes);
    std::pair<bool, QVariant> hasResultOfType(Dice::RESULT_TYPE, ExecutionNode* node, bool notthelast= false) const;
    QList<qreal> scalarResultsFromEachInstruction() const;
    std::pair<QString, QString> finalScalarResult() const;
    QString finalStringResult(std::function<QString(const QString&, const QString&, bool)> colorize,
                              bool removeUnhighlighted= false) const;
    QStringList allFirstResultAsString(bool& hasAlias) const;
    QList<qreal> sumOfDiceResult() const;
    QList<ExportedDiceResult> diceResultFromEachInstruction() const;
    bool hasIntegerResultNotInFirst() const;
    bool hasDiceResult() const;
    bool hasStringResult() const;

    // result
    static QString replaceVariableToValue(const QString& source, QStringList values,
                                          QMap<Dice::ERROR_CODE, QString>& errorMap);
    static QString replacePlaceHolderToValue(const QString& source, const QList<ExportedDiceResult>& list,
                                             bool removeUnhighlighted,
                                             std::function<QString(const QString&, const QString&, bool)> colorize);
    static SubtituteInfo readVariableFromString(const QString& source, int& start);
    static SubtituteInfo readPlaceHolderFromString(const QString& source, int& start);
    static ExportedDiceResult finalDiceResultFromInstruction(ExecutionNode* start);
    static ExportedDiceResult allDiceResultFromInstruction(ExecutionNode* start);
    void addResultInJson(QJsonObject& obj, Dice::RESULT_TYPE type, const QString& key, ExecutionNode* start, bool b);
    void addDiceResultInJson(QJsonObject& obj, ExecutionNode* start,
                             std::function<QString(const QString&, const QString&, bool)> colorize);

    // accessors
    void setComment(const QString& comment);
    QString getComment() const;
    void setHelpPath(const QString& path);
    static QHash<QString, QString> getVariableHash();
    static void setVariableHash(const QHash<QString, QString>& variableHash);
    void setStartNodes(std::vector<ExecutionNode*> nodes);

    // Aliases
    QString convertAlias(QString str);
    void insertAlias(DiceAlias* dice, int i);
    const QList<DiceAlias*>& getAliases() const;
    QList<DiceAlias*>* aliases();
    void setAliases(const QList<DiceAlias*> list);
    void cleanUpAliases();

    static bool readStringResultParameter(QString& str);
    static QString replacePlaceHolderFromJson(const QString& source, const QJsonObject& obj);

private:
    QMap<QString, BooleanCondition::LogicOperator> m_logicOp;
    QMap<QString, ValidatorList::LogicOperation> m_logicOperation;
    QMap<QString, OperationCondition::ConditionOperator> m_conditionOperation;
    std::vector<std::pair<QString, Die::ArithmeticOperator>> m_arithmeticOperation;
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
