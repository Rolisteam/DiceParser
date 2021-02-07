/***************************************************************************
 *	Copyright (C) 2021 by Renaud Guezennec                               *
 *   http://www.rolisteam.org/contact                                      *
 *                                                                         *
 *   This software is free software; you can redistribute it and/or modify *
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
#ifndef SWITCHCASENODE_H
#define SWITCHCASENODE_H

#include <memory>

#include "executionnode.h"
#include "validatorlist.h"

struct CaseInfo
{
    ValidatorList* validatorList;
    ExecutionNode* node;
};
class StringResult;
class SwitchCaseNode : public ExecutionNode
{
public:
    SwitchCaseNode();
    void setStopAtFirt(bool b);

    void run(ExecutionNode* previous= nullptr) override;

    QString toString(bool withLabel) const override;
    qint64 getPriority() const override;
    ExecutionNode* getCopy() const override;

    void insertCase(ExecutionNode* node, ValidatorList* validator);

private:
    std::vector<std::unique_ptr<CaseInfo>> m_branchList;
    StringResult* m_stringResult;
    bool m_stopAtFirst= false;
};

#endif // SWITCHCASENODE_H
