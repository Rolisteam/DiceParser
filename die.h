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
#ifndef DIE_H
#define DIE_H

#include <QList>

class Die
{
public:
    Die();

    void setValue(qint64 r);
    void setFaces(quint64 face);
    void insertRollValue(qint64 r);
    void setSelected(bool b);

    bool isSelected() const;
    qint64 getValue() const;
    QList<qint64> getListValue() const;

    bool hasChildrenValue();


    void roll(bool adding = false);
    void replaceLastValue(qint64 value);


    qint64 getLastRolledValue();
    quint64 getFaces();

    bool hasBeenDisplayed();
    void displayed();

private:
    qint64 m_value;
    QList<qint64> m_rollResult;
    bool m_selected;
    bool m_hasValue;
    bool m_displayStatus;
    quint64 m_faces;
};


#endif // DIE_H
