/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
* http://renaudguezennec.homelinux.org/accueil,3.html                      *
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

/**
 * @mainpage DiceParser
 *
 * The grammar is something like this:
 *
 * Line =: Command | Command;Command
 * Command =: Expression | ScalarOperator Expression | ScalarOperator NodeAction Expression
 * Expression =: number | number Dice | Command
 * Dice =: DiceOperator Number(faces) | DiceOperator List
 * List =: [Word,Number,',']+
 * NodeAction =: @
 * DiceOperator =: [D,L]
 * ScalarOperator =: [x,-,*,x,/]
 * number =: [0-9]+
 * Word =: [A-z]+
 *
 */

class Dice;
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
    enum OptionOperator {KeepAndExplose,Keep,Reroll,Explosing,Sort,Count,RerollAndAdd};

    enum CommandOperator {};

    /**
     * @brief DiceParser default constructor
     */
    DiceParser();

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
    void displayDotTree();
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
    bool readOption(QString&,ExecutionNode* node, bool hasDice = true);

	/**
	 * @brief addRollDiceNode
	 * @param faces
	 * @return
	 */
    DiceRollerNode* addRollDiceNode(qint64 faces,ExecutionNode*);

	/**
	 * @brief readOperand
	 * @param node
	 * @return
	 */
    bool readOperand(QString&,ExecutionNode* & node);
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
	QList<ExecutionNode::ERROR_CODE>  getErrorList();

    bool readInstructionOperator(QChar c);

    bool readNode(QString& str,ExecutionNode* & node);


private:
    QMap<QString,DiceOperator>* m_mapDiceOp;
    QMap<QString,OptionOperator>* m_OptionOp;
    QMap<QString,NodeAction>* m_nodeActionMap;
    QMap<QString,QString>* m_aliasMap;
	QStringList* m_commandList;


    ExecutionNode* m_start;
    ExecutionNode* m_current;
    QString m_command;
    ParsingToolBox* m_parsingToolbox;
};

/**
 * @brief The Dice class is a basic class to store dice. (May not be required any more).
 */
class Dice
{
public:
   DiceParser::DiceOperator m_diceOp;
   int m_faces;
};

#endif // DICEPARSER_H
