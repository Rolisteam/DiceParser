/*************************************************************************
 *   Copyright (C) 2009 by Renaud Guezennec                              *
 *                                                                       *
 *   https://rolisteam.org/                                           *
 *                                                                       *
 *   rolisteam is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published   *
 *   by the Free Software Foundation; either version 2 of the License,   *
 *   or (at your option) any later version.                              *
 *                                                                       *
 *   This program is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with this program; if not, write to the                       *
 *   Free Software Foundation, Inc.,                                     *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           *
 *************************************************************************/
#ifndef LISTSETROLLNODE_H
#define LISTSETROLLNODE_H

#include <QStringList>

#include "executionnode.h"
#include "range.h"
#include "result/diceresult.h"
#include "result/stringresult.h"
/**
 * @brief The ListSetRollNode class is dedicated to pick up item from list.
 */
class ListSetRollNode : public ExecutionNode
{
public:
    ListSetRollNode();
    virtual ~ListSetRollNode();
    virtual void run(ExecutionNode* previous= nullptr);
    virtual QString toString(bool) const;
    virtual qint64 getPriority() const;
    QStringList getList() const;

    void setListValue(QStringList);
    void setUnique(bool);
    void setRangeList(QList<Range>&);
    virtual ExecutionNode* getCopy() const;

private:
    void getValueFromDie(Die* die, QStringList& rollResult);
    void computeFacesNumber(Die* die);

private:
    QStringList m_values;
    DiceResult* m_diceResult;
    StringResult* m_stringResult;
    std::vector<int> m_rangeIndexResult;
    bool m_unique;
    QList<Range> m_rangeList;
};

#endif // LISTSETROLLNODE_H
