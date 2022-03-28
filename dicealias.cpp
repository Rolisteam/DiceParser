/***************************************************************************
 * Copyright (C) 2014 by Renaud Guezennec                                   *
 * https://rolisteam.org/contact                      *
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

QString makeReplament(const QString& pattern, const QString& command, QString cmd)
{
    auto hasPattern= cmd.contains(pattern);
    if(hasPattern)
    {
        auto idxPattern= cmd.indexOf(pattern);
        std::vector<std::pair<int, int>> quotes;

        int pos= 0;
        bool open= true;
        while(pos != -1 && pos < cmd.size())
        {
            auto oldPos= pos;
            pos= cmd.indexOf("\"", pos);
            if(open && pos != -1)
                open= false;
            else if(pos != -1)
            {
                quotes.push_back({oldPos, pos});
            }

            if(pos != -1)
                pos+= 1;
        }
        auto hasQuote= false;
        for(auto range : quotes)
        {
            if(idxPattern < range.second && idxPattern >= range.first)
                hasQuote= true;
        }

        auto hasVariable= cmd.contains("${");
        auto commentPos= cmd.lastIndexOf("#");

        if(!hasQuote && !hasVariable)
        {
            cmd.replace(pattern, command);
        }
        else
        {
            std::vector<int> patternPosList;
            std::vector<std::pair<int, int>> variablePos;

            int pos= 0;
            QRegularExpressionMatch match;
            while(pos != -1)
            {
                auto start= cmd.indexOf(QRegularExpression("\\${\\N+}"), pos, &match);
                if(start >= 0)
                {
                    auto end= start + match.captured().length();
                    variablePos.push_back(std::make_pair(start, end));
                    pos= end + 1;
                }
                else
                {
                    pos= start;
                }
            }

            pos= 0;
            while(pos != -1)
            {
                auto start= cmd.indexOf("\"", pos);
                if(start >= 0)
                {
                    auto end= cmd.indexOf("\"", start + 1);
                    variablePos.push_back(std::make_pair(start, end));
                    pos= end + 1;
                }
                else
                {
                    pos= start;
                }
            }
            pos= 0;
            while((pos= cmd.indexOf(pattern, pos)) && pos != -1)
            {
                bool isInsidePair= false;
                for(auto pair : variablePos)
                {
                    if(!isInsidePair)
                        isInsidePair= (pos > pair.first && pos < pair.second);

                    if(commentPos >= 0 && pos > commentPos)
                        isInsidePair= true;
                }
                if(!isInsidePair)
                    patternPosList.push_back(pos);

                pos+= 1;
            }

            // TODO to be replace by C++14 when it is ready
            for(auto i= patternPosList.rbegin(); i != patternPosList.rend(); ++i)
            {
                cmd.replace(*i, 1, command);
            }
        }
    }
    return cmd;
}

DiceAlias::DiceAlias(QString pattern, QString command, QString comment, bool isReplace, bool isEnable)
    : m_pattern(pattern)
    , m_command(command)
    , m_comment(comment)
    , m_type(isReplace ? REPLACE : REGEXP)
    , m_isEnable(isEnable)
{
}

DiceAlias::~DiceAlias()
{
    // qDebug() << "destructeur of alias!" << this;
}

DiceAlias::DiceAlias(const DiceAlias& alias)
{
    m_command= alias.command();
    m_comment= alias.comment();
    m_pattern= alias.pattern();
    m_isEnable= alias.isEnable();
    m_type= alias.isReplace() ? REPLACE : REGEXP;
}

bool DiceAlias::resolved(QString& str)
{
    if(!m_isEnable)
        return false;

    if((m_type == REPLACE) && (str.contains(m_pattern)))
    {
        str= makeReplament(m_pattern, m_command, str);
        return true;
    }
    else if(m_type == REGEXP)
    {
        QRegularExpression exp(m_command);
        str.replace(exp, m_pattern);
        return true;
    }
    return false;
}

void DiceAlias::setCommand(QString command)
{
    m_command= command;
}

void DiceAlias::setPattern(const QString& pattern)
{
    m_pattern= pattern;
}

void DiceAlias::setType(RESOLUTION_TYPE type)
{
    m_type= type;
}
QString DiceAlias::command() const
{
    return m_command;
}

QString DiceAlias::pattern() const
{
    return m_pattern;
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
        m_type= REGEXP;
    }
}

bool DiceAlias::isEnable() const
{
    return m_isEnable;
}

void DiceAlias::setEnable(bool b)
{
    m_isEnable= b;
}

QString DiceAlias::comment() const
{
    return m_comment;
}

void DiceAlias::setComment(const QString& comment)
{
    m_comment= comment;
}
