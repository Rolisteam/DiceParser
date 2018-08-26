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
#include "dicealias.h"
#include <QRegularExpression>

#include <QDebug>

QString makeReplament(const QString& pattern, const QString& replacement, QString cmd)
{

    // FIXME try to do the same with RegularExpression
    auto hasPattern = cmd.contains(pattern);
    if(hasPattern)
    {
        auto hasVariable = cmd.contains("${");
        auto hasQuote = cmd.contains("\"");

        if(!hasQuote && !hasVariable)
        {
            cmd.replace(pattern, replacement);
        }
        else
        {
            std::vector<int> patternPosList;
            std::vector<std::pair<int,int>> variablePos;

            int pos= 0;
            QRegularExpressionMatch match;
            while(pos!=-1)
            {
                auto start = cmd.indexOf(QRegularExpression("\\${\\N+}"),pos,&match);
                if(start >=0)
                {
                    auto end = start+match.captured().length();
                    variablePos.push_back(std::make_pair(start,end));
                    pos = end+1;
                }
                else
                {
                    pos = start;
                }
            }

            pos = 0;
            while(pos!=-1)
            {
                auto start = cmd.indexOf("\"",pos);
                if(start >= 0)
                {
                    auto end = cmd.indexOf("\"",start+1);
                    variablePos.push_back(std::make_pair(start,end));
                    pos = end+1;
                }
                else
                {
                    pos = start;
                }
            }

            pos= 0;
            while((pos = cmd.indexOf(pattern,pos)) && pos!=-1)
            {
                bool isInsidePair = false;
                for(auto pair : variablePos)
                {
                    if(!isInsidePair)
                        isInsidePair = (pos > pair.first && pos < pair.second);
                }
                if(!isInsidePair)
                    patternPosList.push_back(pos);
                pos+=1;
            }

            // TODO to be replace by C++14 when it is ready
            for (auto i = patternPosList.rbegin(); i != patternPosList.rend(); ++i)
            {
                cmd.replace(*i,1,replacement);
            }
        }
    }
    return cmd;
}




DiceAlias::DiceAlias(QString cmd, QString key, bool isReplace,bool isEnable)
    : m_command(cmd),m_value(key),m_isEnable(isEnable)
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
    if(!m_isEnable)
        return false;

    if((m_type == REPLACE)&&(str.contains(m_command)))
    {
       str = makeReplament(m_command,m_value,str);
       //str.replace(m_command,m_value);
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
QString DiceAlias::getCommand() const
{
    return m_command;
}

QString DiceAlias::getValue() const
{
    return m_value;
}

bool DiceAlias::isReplace() const
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

bool DiceAlias::isEnable() const
{
    return m_isEnable;
}

void DiceAlias::setEnable(bool b)
{
    m_isEnable = b;
}

QString DiceAlias::getComment() const
{
    return m_comment;
}

void DiceAlias::setComment(const QString &comment)
{
    m_comment = comment;
}
