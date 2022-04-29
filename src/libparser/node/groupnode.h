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
#ifndef GROUPNODE_H
#define GROUPNODE_H

#include "node/executionnode.h"
#include "result/scalarresult.h"
#include "result/stringresult.h"
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
    GroupNode(bool complexOutput= false);
    void run(ExecutionNode* previous) override;
    virtual QString toString(bool withLabel) const override;
    virtual qint64 getPriority() const override;
    virtual ExecutionNode* getCopy() const override;

    int getGroupValue() const;
    void setGroupValue(qint64 groupValue);

    QList<DieGroup> getGroup(DieGroup);

protected:
    bool composeWithPrevious(DieGroup previous, qint64 first, qint64 current, DieGroup& addValue);

private:
    ScalarResult* m_scalarResult;
    StringResult* m_stringResult;
    qint64 m_groupValue;
    QList<DieGroup> m_groupsList;
    bool m_complexOutput= false;
};

#endif // GROUPNODE_H
