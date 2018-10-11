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
#ifndef RESULT_H
#define RESULT_H

//#include <Qt>
#include <QString>
#include <QVariant>
/**
 * @brief The Result class
 */
class Result
{
public:
    /**
     * @brief The RESULT_TYPE enum or combinaison
     */
    enum RESULT_TYPE {NONE=0,SCALAR=1,STRING=2,DICE_LIST=4};
    /**
     * @brief Result
     */
    Result();
    /**
     * @brief ~Result
     */
    virtual ~Result();

    /**
     * @brief isScalar
     * @return
     */
    virtual bool hasResultOfType(RESULT_TYPE) const;
    /**
     * @brief getScalar
     * @return
     */
    virtual QVariant getResult(RESULT_TYPE) = 0;
    /**
     * @brief getPrevious
     * @return
     */
    virtual Result* getPrevious();
    /**
     * @brief setPrevious
     */
    virtual void setPrevious(Result*);
    /**
     * @brief isStringResult
     * @return
     */
    virtual bool isStringResult() const;

    virtual void clear();

    /**
     * @brief getStringResult
     * @return
     */
    QString getStringResult();
    /**
     * @brief generateDotTree
     */
    void generateDotTree(QString&);
    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool wl) = 0;
    virtual Result* getCopy() const = 0;
protected:
    int m_resultTypes;/// @brief
    QString m_id;
private:
    Result* m_previous;/// @brief

};

#endif // RESULT_H
