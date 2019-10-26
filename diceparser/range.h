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
#ifndef RANGE_H
#define RANGE_H

#include "validator.h"
#include <Qt>

/**
 * @brief The Range class is validator class to check validity between two values.
 */
class Range : public Validator
{
public:
    Range();
    void setValue(qint64, qint64);
    void setStart(qint64);
    void setEnd(qint64);
    virtual qint64 hasValid(Die* b, bool recursive, bool unlight= false) const override;

    virtual QString toString() override;
    virtual Dice::CONDITION_STATE isValidRangeSize(const std::pair<qint64, qint64>& range) const override;

    bool isFullyDefined() const;
    qint64 getStart() const;
    qint64 getEnd() const;

    void setEmptyRange(bool);
    bool isEmptyRange();

    virtual Validator* getCopy() const override;

private:
    qint64 m_start= 0;
    qint64 m_end= 0;
    bool m_hasEnd;
    bool m_hasStart;
    bool m_emptyRange;
};

#endif // RANGE_H
