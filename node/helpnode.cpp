/***************************************************************************
    *   Copyright (C) 2015 by Renaud Guezennec                                *
    *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
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
    : m_path("https://github.com/obiwankennedy/DiceParser/blob/master/HelpMe.md")
{
    m_result = new StringResult();
}
void HelpNode::run(ExecutionNode* previous)
{
	m_previousNode = previous;
    StringResult* txtResult = dynamic_cast<StringResult*>(m_result);
    txtResult->setHighLight(false);

    if(NULL != previous)
    {
        if(previous->getResult() == NULL)
        {
            txtResult->setText(toString());

        }
        else
        {
            txtResult->setText(previous->getHelp());
        }
        m_result->setPrevious(previous->getResult());
    }

    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}
QString HelpNode::toString() const
{
    return QObject::tr("Rolisteam Dice Parser:\nFull documentation at: %1 \n").arg(m_path);
}

qint64 HelpNode::getPriority() const
{
    return 0;
}
void HelpNode::setHelpPath(QString path)
{
    m_path = path;
}
