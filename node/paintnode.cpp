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

ColorItem::ColorItem(QString str, int val)
    : m_colorNumber(val),m_color(str)
{

}

int ColorItem::colorNumber() const
{
    return m_colorNumber;
}

void ColorItem::setColorNumber(int colorNumber)
{
    m_colorNumber = colorNumber;
}

QString ColorItem::color() const
{
    return m_color;
}

void ColorItem::setColor(const QString &color)
{
    m_color = color;
}


///////////////////////////////////
/// @brief PainterNode::PainterNode
///////////////////////////////////

PainterNode::PainterNode()
    : ExecutionNode()
{
    m_result = nullptr;
    m_nextNode = nullptr;
}


PainterNode::~PainterNode()
{
    m_result = nullptr;
}


void PainterNode::run(ExecutionNode* previous)
{
    m_previousNode = previous;
    if(nullptr==previous)
    {
        m_errors.insert(ExecutionNode::NO_PREVIOUS_ERROR, QObject::tr("No previous node before Paint operator"));
        return;
    }
    Result* previousResult = previous->getResult();
    //m_result = previousResult;
    DiceResult* previousDiceResult = dynamic_cast<DiceResult*>(previousResult);
    if(nullptr!=previousDiceResult)
    {
        QList<Die*> diceList=previousDiceResult->getResultList();
        int pastDice=0;
        for(ColorItem& item : m_colors)
        {
            int current=item.colorNumber();
            QList<Die*>::iterator it;
            for(it = diceList.begin()+pastDice; it != diceList.end() && current>0 ; ++it)
            {
                (*it)->setColor(item.color());
                --current;
                ++pastDice;
            }
        }
    }
    if(nullptr!=m_nextNode)
    {
        m_nextNode->run(previous);
    }
}
Result* PainterNode::getResult()
{
     return (nullptr != m_previousNode) ? m_previousNode->getResult() :  nullptr ;
}

QString PainterNode::toString(bool wl) const
{
    if(wl)
    {
        return QString("%1 [label=\"PainterNode\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}


qint64 PainterNode::getPriority() const
{
    return 4;
}


void PainterNode::insertColorItem(QString color, int value)
{
    ColorItem item(color,value);
    m_colors.append(item);
}
ExecutionNode* PainterNode::getCopy() const
{
    PainterNode* node = new PainterNode();
    if(nullptr!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
