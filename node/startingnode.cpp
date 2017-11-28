/***************************************************************************
    *   Copyright (C) 2015 by Renaud Guezennec                                *
    *   http://www.rolisteam.org/contact                   *
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
#include "startingnode.h"
#include <QDebug>

StartingNode::StartingNode()
{

}
void StartingNode::run(ExecutionNode*)
{
    m_previousNode = nullptr;
    if(nullptr!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}
QString StartingNode::toString(bool withlabel) const
{
    if(withlabel)
    {
        return QString("%1 [label=\"StartingNode\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}


qint64 StartingNode::getPriority() const
{
    qint64 priority=0;
    if(nullptr!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }
    return priority;
}
ExecutionNode* StartingNode::getCopy() const
{
    StartingNode* node = new StartingNode();
    if(nullptr!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;

}
