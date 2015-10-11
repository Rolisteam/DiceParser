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
#include "numbernode.h"

NumberNode::NumberNode()
    : m_scalarResult(new ScalarResult())
{
    m_result = m_scalarResult;
}
void NumberNode::run(ExecutionNode* previous)
{
	m_previousNode = previous;
    if(NULL!=previous)
    {
        m_result->setPrevious(previous->getResult());
    }
    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}

void NumberNode::setNumber(qint64 a)
{
    m_scalarResult->setValue(a);
    m_number = a;
}
QString NumberNode::toString(bool withLabel) const
{
    if(withLabel)
	{
		return QString("%1 [label=\"NumberNode %2\"]").arg(m_id).arg(m_number);
	}
	else
	{
		return m_id;
	}
}
qint64 NumberNode::getPriority() const
{
    qint64 priority=0;
    if(NULL!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
