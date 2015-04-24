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
#include "dicealias.h"
#include <QRegularExpression>

DiceAlias::DiceAlias(QString cmd, QString key, bool isReplace)
    : m_command(cmd),m_value(key)
{
    if(isReplace)
    {
        m_type = REPLACE;
    }
    else
    {
        m_type = REGEXP;
    }
}

DiceAlias::~DiceAlias()
{

}

bool DiceAlias::resolved(QString & str)
{
    if((m_type == REPLACE)&&(str.contains(m_command)))
    {
       str.replace(m_command,m_value);
       return true;
    }
    else if(m_type == REGEXP)
    {
        QRegularExpression  exp(m_command);
        str.replace(exp,m_value);
        return true;
    }
    return false;
}

void DiceAlias::setCommand(QString key)
{
    m_command = key;
}

void DiceAlias::setValue(QString value)
{
    m_value = value;
}

void DiceAlias::setType(RESOLUTION_TYPE type)
{
    m_type = type;
}
QString DiceAlias::getCommand()
{
    return m_command;
}

QString DiceAlias::getValue()
{
    return m_value;
}

bool DiceAlias::isReplace()
{
    return (m_type == REPLACE) ? true : false;
}


void DiceAlias::setReplace(bool b)
{
    if(b)
    {
        m_type= REPLACE;
    }
    else
    {
        m_type = REGEXP;
    }
}
