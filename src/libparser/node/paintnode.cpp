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
#include "paintnode.h"

ColorItem::ColorItem(QString str, int val) : m_colorNumber(val), m_color(str) {}

int ColorItem::colorNumber() const
{
    return m_colorNumber;
}

void ColorItem::setColorNumber(int colorNumber)
{
    m_colorNumber= colorNumber;
}

QString ColorItem::color() const
{
    return m_color;
}

void ColorItem::setColor(const QString& color)
{
    m_color= color;
}

///////////////////////////////////
/// @brief PainterNode::PainterNode
///////////////////////////////////

PainterNode::PainterNode() : ExecutionNode("%1 [label=\"PainterNode\"]")
{
    m_nextNode= nullptr;
}

PainterNode::~PainterNode()
{
    m_result= nullptr;
}

void PainterNode::run(ExecutionNode* previous)
{
    if(isValid(!previous, Dice::ERROR_CODE::NO_PREVIOUS_ERROR, tr("No Previous node")))
        return;
    m_previousNode= previous;

    Result* previousResult= previous->getResult();
    if(isValid(!previousResult, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid result")))
        return;

    m_diceResult= dynamic_cast<DiceResult*>(previousResult->getCopy());
    if(isValid(!m_diceResult, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid Dice result")))
        return;

    QList<Die*> diceList= m_diceResult->getResultList();
    int pastDice= 0;
    for(ColorItem& item : m_colors)
    {
        int current= item.colorNumber();
        QList<Die*>::iterator it;
        for(it= diceList.begin() + pastDice; it != diceList.end() && current > 0; ++it)
        {
            (*it)->setColor(item.color());
            --current;
            ++pastDice;
        }
    }
    m_diceResult->setPrevious(previousResult);
    m_result= m_diceResult;
}

qint64 PainterNode::getPriority() const
{
    return 4;
}

void PainterNode::insertColorItem(QString color, int value)
{
    ColorItem item(color, value);
    m_colors.append(item);
}
ExecutionNode* PainterNode::getCopy() const
{
    PainterNode* node= new PainterNode();
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
