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
#ifndef DICEALIAS_H
#define DICEALIAS_H

#include <QString>
/**
 * @brief The DiceAlias class is dedicated to store aliases, alias is mainly two QString. The Alias and its replacement.
 * The replacement can be a simple QString or a RegExp.
 */
class DiceAlias
{
public:
    enum RESOLUTION_TYPE
    {
        REPLACE,
        REGEXP
    };
    /**
     * @brief DiceAlias
     * @param cmd
     * @param key
     * @param isReplace
     */
    DiceAlias(QString pattern, QString remplacement, bool isReplace= true, bool isEnable= true);
    /**
     * @brief ~DiceAlias
     */
    virtual ~DiceAlias();
    /**
     * @brief resolved
     * @param str
     * @return
     */
    bool resolved(QString& str);
    /**
     * @brief setCommand
     * @param key
     */
    void setCommand(QString key);
    /**
     * @brief setValue
     * @param value
     */
    void setValue(QString value);
    /**
     * @brief setType
     */
    void setType(RESOLUTION_TYPE);

    /**
     * @brief getCommand
     * @return
     */
    QString getCommand() const;
    /**
     * @brief getValue
     * @return
     */
    QString getValue() const;
    /**
     * @brief isReplace
     * @return
     */
    bool isReplace() const;
    /**
     * @brief setReplace
     */
    void setReplace(bool);

    /**
     * @brief isEnable
     * @return
     */
    bool isEnable() const;
    /**
     * @brief setEnable
     * @param b
     */
    void setEnable(bool b);
    /**
     * @brief getComment
     * @return
     */
    QString getComment() const;
    /**
     * @brief setComment
     * @param comment
     */
    void setComment(const QString& comment);

private:
    QString m_command;
    QString m_value;
    QString m_comment;
    RESOLUTION_TYPE m_type;
    bool m_isEnable;
};

#endif // DICEALIAS_H
