/***************************************************************************
    *   Copyright (C) 2016 by Renaud Guezennec                                *
    *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
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
    IfNode();
    virtual ~IfNode();
    virtual void run(ExecutionNode* previous = NULL);
    virtual void setValidator(Validator* );
    virtual void setInstructionTrue(ExecutionNode*);
    virtual void setInstructionFalse(ExecutionNode*);
    virtual QString toString(bool )const;
    virtual qint64 getPriority() const;

protected:
    DiceResult* m_diceResult;
    Validator* m_validator;

    ExecutionNode* m_true;
    ExecutionNode* m_false;
};
#endif
