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
#ifndef GROUPNODE_H
#define GROUPNODE_H

#include "node/executionnode.h"
#include "result/scalarresult.h"
// typedef QList<qint64> DieGroup;
#include <QList>

class DieGroup : public QList<qint64>
{
public:
    int getSum() const;
    void removeValue(DieGroup);

    int getLost() const;

    qint64 getExceptedValue() const;
    void setExceptedValue(qint64 exceptedValue);

private:
    qint64 m_exceptedValue= 0;
};
/**
 * @brief The GroupNode class is an ExecutionNode.
 */
class GroupNode : public ExecutionNode
{
public:
    GroupNode();
    void run(ExecutionNode* previous);
    virtual QString toString(bool withLabel) const;
    virtual qint64 getPriority() const;
    virtual ExecutionNode* getCopy() const;

    int getGroupValue() const;
    void setGroupValue(qint64 groupValue);

    QList<DieGroup> getGroup(DieGroup);

protected:
    bool composeWithPrevious(DieGroup previous, qint64 first, qint64 current, DieGroup& addValue);

private:
    ScalarResult* m_scalarResult;
    qint64 m_groupValue;
    QList<DieGroup> m_groupsList;
};

#endif // GROUPNODE_H
