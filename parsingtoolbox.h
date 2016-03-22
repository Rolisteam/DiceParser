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
#ifndef PARSINGTOOLBOX_H
#define PARSINGTOOLBOX_H

#include <QMap>

#include "node/executionnode.h"
#include "node/dicerollernode.h"
#include "booleancondition.h"
#include "operationcondition.h"
#include "compositevalidator.h"
#include "range.h"
#include "node/scalaroperatornode.h"
#include "node/paintnode.h"
/**
 * @brief The ParsingToolBox is gathering many useful methods for dice parsing.
 * Its goal is to make the diceparser a bit lighter.
 */
class ParsingToolBox
{
public:
    enum LIST_OPERATOR {NONE,UNIQUE};

	/**
	 * @brief ParsingToolBox
	 */
    ParsingToolBox();
    /**
     * @brief ParsingToolBox
     * @param data
     */
    ParsingToolBox(const ParsingToolBox & data);
	/**
	 * @brief ~ParsingToolBox
	 */
	virtual ~ParsingToolBox();
	/**
	 * @brief addSort
	 * @param e
	 * @param b
	 * @return
	 */
    ExecutionNode* addSort(ExecutionNode* e,bool b);
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
    bool readLogicOperator(QString& str,BooleanCondition::LogicOperator& op);
	/**
	 * @brief readValidator
	 * @param str
	 * @return
	 */
    Validator* readValidator(QString& str);
    /**
     * @brief readCompositeValidator
     * @param str
     * @return
     */
    Validator* readCompositeValidator(QString& str);

    /**
     * @brief readNumber read number in the given str and remove from the string the read character.
     * @param str the command line
     * @param myNumber reference to the found number
     * @return true, succeed to read number, false otherwise.
     */
    static bool readNumber(QString&  str, qint64& myNumber);


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
     * @brief readList
     * @param str
     * @param list
     * @return
     */
    bool readList(QString& str,QStringList& list, QList<Range>& ranges);
    /**
     * @brief isValidValidator
     * @param previous
     * @param val
     * @return
     */
    bool isValidValidator(ExecutionNode* previous, Validator* val);
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
    bool readDiceRange(QString& str,qint64& start, qint64& end);
    /**
     * @brief readListOperator
     * @param str
     * @return
     */
    static LIST_OPERATOR  readListOperator(QString& str);

    void readProbability(QStringList& str,QList<Range>& ranges);

    bool readLogicOperation(QString& str,CompositeValidator::LogicOperation& op);

    bool readDiceLogicOperator(QString &str, OperationCondition::ConditionOperator &op);

    bool readArithmeticOperator(QString& str, ScalarOperatorNode::ArithmeticOperator& op);

    static void readPainterParameter(PainterNode *painter, QString &str);
private:
        QMap<QString,BooleanCondition::LogicOperator>* m_logicOp;
        QMap<QString,CompositeValidator::LogicOperation>* m_logicOperation;
        QMap<QString,OperationCondition::ConditionOperator>* m_conditionOperation;
        QHash<QString,ScalarOperatorNode::ArithmeticOperator>* m_arithmeticOperation;
};

#endif // PARSINGTOOLBOX_H
