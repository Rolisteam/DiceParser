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
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <Qt>
#include "die.h"
#include <QString>
/**
 * @brief The Validator class is an abstract class for checking the validity of dice for some
 * operator.
 */
class Validator
{
public:
    /**
     * @brief Validator
     */
    Validator();
    /**
     * @brief hasValid
     * @param b
     * @param recursive
     * @param unlight
     * @return
     */
    virtual qint64 hasValid(Die* b,bool recursive,bool unlight = false) const = 0 ;
    /**
     * @brief toString
     * @return
     */
    virtual QString toString()=0;
    /**
     * @brief getValidRangeSize
     * @param faces
     * @return
     */
    virtual quint64 getValidRangeSize(quint64 faces) const = 0 ;
};

#endif // VALIDATOR_H
