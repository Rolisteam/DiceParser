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
#ifndef DICERESULT_H
#define DICERESULT_H
#include <QList>

#include "die.h"
#include "result.h"
/**
 * @brief The DiceResult class
 */
class DiceResult : public Result
{
public:
    /**
     * @brief DiceResult
     */
    DiceResult();
	/**
	 * @brief ~DiceResult
	 */
	virtual ~DiceResult();

    /**
     * @brief getResultList
     * @return
     */
    QList<Die*>& getResultList();
    /**
     * @brief insertResult
     */
    void insertResult(Die*);

    /**
     * @brief setResultList
     * @param list
     */
    void setResultList(QList<Die*> list);

    /**
     * @brief getScalar
     * @return
     */
    virtual QVariant getResult(RESULT_TYPE);
    /**
     * @brief toString
     * @return
     */
	virtual QString toString(bool wl);
    /**
     * @brief isHomogeneous
     */
    bool isHomogeneous() const;
    /**
     * @brief setHomogeneous
     */
    void setHomogeneous(bool);

    Die::ArithmeticOperator getOperator() const;
    void setOperator(const Die::ArithmeticOperator & dieOperator);    
    bool contains(Die *die, const std::function<bool (const Die *, const Die *)> equal);

    virtual Result* getCopy() const;
private:
    qreal getScalarResult();
private:
    QList<Die*> m_diceValues;
    bool m_homogeneous;
    Die::ArithmeticOperator m_operator;
};

#endif // DICERESULT_H
