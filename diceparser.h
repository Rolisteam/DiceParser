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

#include <QString>
#include <QMap>

#include "node/executionnode.h"
#include "node/dicerollernode.h"

#include "validator.h"
#include "range.h"
#include "booleancondition.h"
#include "parsingtoolbox.h"
#include "dicealias.h"
#include "highlightdice.h"

typedef QList<HighLightDice > ListDiceResult;
typedef QMap<int,ListDiceResult > ExportedDiceResult;

class ExploseDiceNode;
/**
 * @page DiceParser Dice Parser
 *
 * @section Intro Introduction
 * Diceparser is the software component dedicated to compute dice command in rolisteam.<br/>
 *
 * @section grammar The Grammar
 *
 * The grammar is something like this:
 *
 * Line =: Command | Command;Command<br/>
 * Command =: Expression | ScalarOperator Expression | ScalarOperator NodeAction Expression<br/>
 * Expression =: number | number Dice | Command<br/>
 * Dice =: DiceOperator Number(faces) | DiceOperator List<br/>
 * List =: [Word[range|percentage],Number[range|percentage],',']+<br/>
 * Persentage =: Number <br/>
 * NodeAction =: @<br/>
 * DiceOperator =: [D,L]<br/>
 * ScalarOperator =: [x,-,*,x,/]<br/>
 * number =: [0-9]+<br/>
 * Word =: [A-z]+<br/>
 *
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
	enum DiceOperator {D,L};

    /**
     * @brief The DiceSymbol enum
     */
    enum NodeAction {JumpBackward};
    /**
     * @brief The OptionOperator enum gathering all options  availables for result.
     */
    enum OptionOperator {KeepAndExplose,Keep,Reroll,Explosing,Sort,Count,RerollAndAdd,Merge,ifOperator,Painter};
    /**
     * @brief The CommandOperator enum
     */
    enum CommandOperator {};

    /**
     * @brief DiceParser default constructor
     */
    DiceParser();
	/**
	 * @brief ~DiceParser
	 */
	virtual ~DiceParser();

    /**
     * @brief parseLine, method to call for starting the dice roll. It will parse the command and run the execution tree.
     * @param str dice command
     * @return bool every thing is fine or not
     */
    bool parseLine(QString str);


    /**
     * @brief Start running the execution tree
     *
     */
    void Start();

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
    bool readExpression(QString& str,ExecutionNode* & node);
    /**
     * @brief displayDotTree
     */
    void writeDownDotTree(QString filepath);
    /**
     * @brief getLastIntegerResult
     * @return
     */
    qreal getLastIntegerResult();
    /**
     * @brief getSumOfDiceResult
     * @return
     */
    qreal getSumOfDiceResult();
    /**
     * @brief getLastDiceResult
     * @return
     */
    void getLastDiceResult(ExportedDiceResult& diceValues,bool& homogeneous);
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
    QString getStringResult();
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
    QMap<ExecutionNode::DICE_ERROR_CODE,QString> getErrorMap();
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

    bool hasSeparator()const;
    bool readIfInstruction(QString &str, ExecutionNode* &trueNode, ExecutionNode* &falseNode);
    /**
     * @brief setVariableDictionary
     * @param variables
     */
    void setVariableDictionary(QHash<QString,QString>* variables);
private:

    /**
     * @brief readDice
     * @param str
     * @return
     */
    bool readDice(QString&  str,ExecutionNode* & node);
    /**
     * @brief readDiceOperator
     * @return
     */
    bool readDiceOperator(QString&,DiceOperator&);
    /**
     * @brief readDiceExpression
     * @param node
     * @return
     */
    bool readDiceExpression(QString&,ExecutionNode*  & node);
    /**
     * @brief readOperator
     * @return
     */
    bool readOperator(QString&,ExecutionNode* previous);
    /**
     * @brief setCurrentNode
     * @param node
     */
    ExecutionNode* getLatestNode(ExecutionNode* node);
    /**
     * @brief DiceParser::readCommand
     * @param str
     * @param node
     * @return
     */
    bool readCommand(QString& str,ExecutionNode* & node);

    /**
     * @brief readOption
     */
    bool readOption(QString&,ExecutionNode* node, bool hasDice = true);//OptionOperator& option,

	/**
	 * @brief addRollDiceNode
	 * @param faces
	 * @return
	 */
    DiceRollerNode* addRollDiceNode(qint64 faces,ExecutionNode*);
    /**
     * @brief addExploseDiceNode
     * @param faces
     * @param previous
     * @return
     */
    ExploseDiceNode* addExploseDiceNode(qint64 faces,ExecutionNode* previous);
	/**
	 * @brief readOperand
	 * @param node
	 * @return
	 */
    bool readOperand(QString&,ExecutionNode* & node);

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
    bool readNode(QString& str,ExecutionNode* & node);

    /**
     * @brief getLeafNode
     * @return
     */
    ExecutionNode* getLeafNode();

    /**
     * @brief hasResultOfType
     * @param notthelast
     * @return
     */
    bool hasResultOfType(Result::RESULT_TYPE,bool notthelast = false);


private:
    QMap<QString,DiceOperator>* m_mapDiceOp;
    QMap<QString,OptionOperator>* m_OptionOp;
    QMap<QString,NodeAction>* m_nodeActionMap;
    QList<DiceAlias*>* m_aliasList;
	QStringList* m_commandList;

    QMap<ExecutionNode::DICE_ERROR_CODE,QString> m_errorMap;
    QMap<ExecutionNode::DICE_ERROR_CODE,QString> m_warningMap;


    ExecutionNode* m_start;
    ExecutionNode* m_current;
    QString m_command;
    ParsingToolBox* m_parsingToolbox;
    QString m_helpPath;
    bool m_currentTreeHasSeparator;
    bool readBlocInstruction(QString &str, ExecutionNode *&resultnode);
};

#endif // DICEPARSER_H
