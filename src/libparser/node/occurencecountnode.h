/***************************************************************************
 *   Copyright (C) 2018 by Renaud Guezennec                                *
 *   https://rolisteam.org/contact                                      *
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
#ifndef OCCURENCECOUNTNODE_H
#define OCCURENCECOUNTNODE_H

#include "executionnode.h"

class ValidatorList;
class StringResult;
class DiceResult;
class OccurenceCountNode : public ExecutionNode
{
public:
    OccurenceCountNode();

    void run(ExecutionNode* previous= nullptr);
    virtual QString toString(bool withLabel) const;

    ExecutionNode* getCopy() const;
    qint64 getPriority() const;

    qint64 getWidth() const;
    void setWidth(const qint64& width);

    qint64 getHeight() const;
    void setHeight(const qint64& height);

    ValidatorList* getValidatorList() const;
    void setValidatorList(ValidatorList* validator);

private:
    void runForStringResult(const std::map<qint64, qint64>& mapOccurence, QVector<qint64>& vec);
    void runForDiceResult(const std::map<qint64, qint64>& mapOccurence);

private:
    qint64 m_width= 1;
    qint64 m_height= 0;
    ValidatorList* m_validatorList= nullptr;
    StringResult* m_stringResult= nullptr;
    DiceResult* m_diceResult= nullptr;
};

#endif // OCCURENCECOUNTNODE_H
