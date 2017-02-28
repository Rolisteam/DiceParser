/***************************************************************************
    *   Copyright (C) 2015 by Renaud Guezennec                                *
    *   http:://www.rolisteam.org/contact                                     *
    *                                                                         *
    *   rolisteam is free software; you can redistribute it and/or modify     *
    *   it under the terms of the GNU General Public License as published by  *
    *   the Free Software Foundation; either version 2 of the License, or     *
    *   (at your option) any later version.                                   *
    *                                                                         *
    *   This program is distributed in the hope that it will be useful,       *
    *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
    *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
    *   GNU General Public License for more details.                          *
    *                                                                         *
    *   You should have received a copy of the GNU General Public License     *
    *   along with this program; if not, write to the                         *
    *   Free Software Foundation, Inc.,                                       *
    *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
    ***************************************************************************/
#ifndef IFNODE_H
#define IFNODE_H

#include "executionnode.h"
#include "result/diceresult.h"
#include "validator.h"
#include <QDebug>

/**
 * @brief The ifNode class explose dice while is valid by the validator.
 */
class IfNode : public ExecutionNode
{
public:
    /**
     * @brief The ConditionType enum
     */
    enum ConditionType {OnEach,OneOfThem,AllOfThem,OnScalar};
    /**
     * @brief IfNode
     */
    IfNode();
    /**
     * @brief ~IfNode
     */
    virtual ~IfNode();
    /**
     * @brief run
     * @param previous
     */
    virtual void run(ExecutionNode* previous = NULL);
    /**
     * @brief setValidator
     */
    virtual void setValidator(Validator* );
    /**
     * @brief setInstructionTrue
     */
    virtual void setInstructionTrue(ExecutionNode*);
    /**
     * @brief setInstructionFalse
     */
    virtual void setInstructionFalse(ExecutionNode*);
    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool )const;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const;


    /**
     * @brief generateDotTree
     */
    virtual void generateDotTree(QString&);

    /**
     * @brief getCopy
     * @return
     */
    virtual ExecutionNode *getCopy() const;
    /**
     * @brief getConditionType
     * @return
     */
    ConditionType getConditionType() const;

    /**
     * @brief setConditionType
     * @param conditionType
     */
    void setConditionType(const IfNode::ConditionType &conditionType);

protected:
    ExecutionNode *getLeafNode(ExecutionNode *node);

protected:
    Validator* m_validator;
    ConditionType m_conditionType;

    ExecutionNode* m_true;
    ExecutionNode* m_false;
};
#endif
