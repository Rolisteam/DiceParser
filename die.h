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
#ifndef DIE_H
#define DIE_H

#include <QList>
#include <QString>

#include <memory>
#include <deque>
#include <random>

struct RandomGenerator {
    using result_type = uint_fast32_t;
    virtual result_type operator() () = 0;
    virtual result_type min () = 0;
    virtual result_type max () = 0;
};

struct DefaultRandomGenerator: public RandomGenerator {
public:
    result_type operator() () override {
        return m_rng();
    }
    result_type min () override {
        return m_rng.min();
    }
    result_type max () override {
        return m_rng.max();
    }
private:
    static std::mt19937 m_rng;
};

struct CustomRandomGenerator: public RandomGenerator {
public:
    CustomRandomGenerator(result_type maxDiceValue, std::deque<result_type> values): m_max{maxDiceValue}, m_values{move(values)} {}
    result_type operator() () override {
        auto val = m_values.front();
        m_values.pop_front();
        return val - 1;
    }
    result_type min () override {
        return 0;
    }
    result_type max () override {
        return m_max;
    }
private:
    result_type m_max;
    std::deque<result_type> m_values;
};

/**
 * @brief The Die class implements all methods required from a die. You must set the Faces first, then you can roll it and roll it again, to add or replace the previous result.
 */
class Die
{
public:
    /**
     * @brief The ArithmeticOperator enum
     */
    enum ArithmeticOperator {PLUS,MINUS,DIVIDE,MULTIPLICATION};
    /**
     * @brief Die
     * @param max: Roll the die between min and max (inclusive)
     */
    Die(qint64 min, qint64 max);

    /**
     * @brief setValue
     * @param r
     */
    void setValue(qint64 r);
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
    quint64 getFaces() const;
    /**
     * @brief hasBeenDisplayed
     * @return
     */
    bool hasBeenDisplayed() const;
    /**
     * @brief displayed
     */
    void displayed();
    /**
     * @brief setHighlighted
     */
    void setHighlighted(bool);
    /**
     * @brief isHighlighted
     * @return
     */
    bool isHighlighted() const;

    QString getColor() const;
    void setColor(const QString &color);

    qint64 getMaxValue() const;

    Die::ArithmeticOperator getOp() const;
    void setOp(const Die::ArithmeticOperator &op);

private:
    qint64 m_value;
    QList<qint64> m_rollResult;
    bool m_selected;
    bool m_hasValue;
    bool m_displayStatus;
    bool m_highlighted;
    qint64 m_min;
    qint64 m_max;
    QString m_color;
    Die::ArithmeticOperator m_op;

    static std::shared_ptr<RandomGenerator> m_rng;

private:
    static void setRandomGenerator(std::shared_ptr<RandomGenerator>&& r);
public:
    template<class RandomGenerator>
    static void setRandomGenerator(RandomGenerator& r) {
        setRandomGenerator(std::make_shared<RandomGenerator>(r));
    }
};

#endif // DIE_H
