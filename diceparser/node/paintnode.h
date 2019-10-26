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
#ifndef PAINTERNODE_H
#define PAINTERNODE_H

#include "executionnode.h"
#include "result/diceresult.h"
#include <QString>

class ColorItem
{
public:
    ColorItem(QString str, int val);
    int colorNumber() const;
    void setColorNumber(int colorNumber);

    QString color() const;
    void setColor(const QString& color);

private:
    int m_colorNumber;
    QString m_color;
};
/**
 * @brief The PainterNode class means to manage color attribute of dice.
 */
class PainterNode : public ExecutionNode
{
public:
    PainterNode();
    virtual ~PainterNode();
    virtual void run(ExecutionNode* previous= nullptr);
    Result* getResult();
    virtual QString toString(bool) const;
    virtual qint64 getPriority() const;
    void insertColorItem(QString color, int value);
    virtual ExecutionNode* getCopy() const;

protected:
    QList<ColorItem> m_colors;
    DiceResult* m_diceResult= nullptr;
};

#endif
