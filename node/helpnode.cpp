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
#include "helpnode.h"

HelpNode::HelpNode()
    : m_path("https://github.com/Rolisteam/DiceParser/blob/master/HelpMe.md")
{
    m_result = new StringResult();
}
void HelpNode::run(ExecutionNode* previous)
{
    m_previousNode = previous;
    StringResult* txtResult = dynamic_cast<StringResult*>(m_result);
    txtResult->setHighLight(false);

    if((nullptr == previous)&&(txtResult != nullptr))
    {
        txtResult->setText(QObject::tr("Rolisteam Dice Parser:\nFull documentation at: %1").arg(m_path));
        m_result->setPrevious(nullptr);
    }
    else if(nullptr != previous)
    {
        txtResult->setText(previous->getHelp());
        m_result->setPrevious(previous->getResult());
    }

    if(nullptr!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}
QString HelpNode::toString(bool wl) const
{
      if(wl)
      {
              return QString("%1 [label=\"Rolisteam Dice Parser:\nFull documentation at: %2\"]").arg(m_id).arg(m_path);
      }
      else
      {
              return m_id;
      }
}

qint64 HelpNode::getPriority() const
{
    return 0;
}
void HelpNode::setHelpPath(QString path)
{
    m_path = path;
}

ExecutionNode* HelpNode::getCopy() const
{
    HelpNode* node = new HelpNode();
    if(nullptr!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
