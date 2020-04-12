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
#ifndef DICERESULT_H
#define DICERESULT_H
#include <QList>
#include <functional>

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
    virtual ~DiceResult() override;

    /**
     * @brief getResultList
     * @return
     */
    virtual QList<Die*>& getResultList();
    /**
     * @brief insertResult
     */
    virtual void insertResult(Die*);

    /**
     * @brief setResultList
     * @param list
     */
    virtual void setResultList(QList<Die*> list);

    /**
     * @brief getScalar
     * @return
     */
    virtual QVariant getResult(Dice::RESULT_TYPE) override;
    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool wl) override;
    /**
     * @brief isHomogeneous
     */
    virtual bool isHomogeneous() const;
    /**
     * @brief setHomogeneous
     */
    virtual void setHomogeneous(bool);

    Die::ArithmeticOperator getOperator() const;
    void setOperator(const Die::ArithmeticOperator& dieOperator);
    bool contains(Die* die, const std::function<bool(const Die*, const Die*)> equal);

    void clear() override;

    virtual Result* getCopy() const override;

protected:
    qreal getScalarResult();

protected:
    QList<Die*> m_diceValues;
    bool m_homogeneous;
    Die::ArithmeticOperator m_operator= Die::ArithmeticOperator::PLUS;
};
Q_DECLARE_METATYPE(QList<Die*>)
#endif // DICERESULT_H
