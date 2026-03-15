/***************************************************************************
 *   Copyright (C) 2015 by Renaud Guezennec                                *
 *   https://rolisteam.org/contact                   *
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
#include "helpnode.h"

HelpNode::HelpNode() : m_path("https://invent.kde.org/rolisteam/rolisteam-diceparser/-/blob/master/HelpMe.md")
{
    m_stringResult= new StringResult();
    m_result= m_stringResult;
    m_stringResult->setPrevious(nullptr);
}
void HelpNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    m_stringResult->setHighLight(false);

    if(!previous)
    {
        m_stringResult->addText(QObject::tr("Rolisteam Dice Parser:\n"
                                            "\n"
                                            "Example (with ! as prefix):\n"
                                            "!2d6\n"
                                            "!1d20\n"
                                            "!6d10e10k3 (L5R)\n"
                                            "\n"
                                            "Full documentation at: %1")
                                    .arg(m_path));
    }
    else
    {
        m_stringResult->addText(previous->getHelp());
        m_result->setPrevious(previous->getResult());
    }
    m_stringResult->finished();
}
QString HelpNode::toString(bool wl) const
{
    return wl ? QString("%1 [label=\"%3 : %2\"]")
                    .arg(m_id, m_path, tr("Rolisteam Dice Parser:\nFull documentation at")) :
                ExecutionNode::toString(wl);
}

qint64 HelpNode::getPriority() const
{
    return 0;
}
void HelpNode::setHelpPath(QString path)
{
    m_path= path;
}

ExecutionNode* HelpNode::getCopy() const
{
    HelpNode* node= new HelpNode();
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
