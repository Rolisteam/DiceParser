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

/**
 * @brief The Die class
 */
class Die
{
public:
    /**
     * @brief Die
     */
    Die();

    /**
     * @brief setValue
     * @param r
     */
    void setValue(qint64 r);
    /**
     * @brief setFaces
     * @param face
     */
    void setFaces(quint64 face);
    /**
     * @brief insertRollValue
     * @param r
     */
    void insertRollValue(qint64 r);
    /**
     * @brief setSelected
     * @param b
     */
    void setSelected(bool b);
    /**
     * @brief isSelected
     * @return
     */
    bool isSelected() const;
    /**
     * @brief getValue
     * @return
     */
    qint64 getValue() const;
    /**
     * @brief getListValue
     * @return
     */
    QList<qint64> getListValue() const;
    /**
     * @brief hasChildrenValue
     * @return
     */
    bool hasChildrenValue();

    /**
     * @brief roll
     * @param adding
     */
    void roll(bool adding = false);
    /**
     * @brief replaceLastValue
     * @param value
     */
    void replaceLastValue(qint64 value);

    /**
     * @brief getLastRolledValue
     * @return
     */
    qint64 getLastRolledValue();
    /**
     * @brief getFaces
     * @return
     */
    quint64 getFaces();
    /**
     * @brief hasBeenDisplayed
     * @return
     */
    bool hasBeenDisplayed();
    /**
     * @brief displayed
     */
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
