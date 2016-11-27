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
#ifndef HELPNODE_H
#define HELPNODE_H
#include "executionnode.h"

#include <QObject>
#include <QString>

#include "result/stringresult.h"

/**
 * @brief The HelpNode class
 */
class HelpNode : public ExecutionNode
{
public:
    /**
     * @brief HelpNode
     */
    HelpNode();
    /**
     * @brief run
     * @param previous
     */
    void run(ExecutionNode* previous);
    /**
     * @brief toString
     * @return
     */
	virtual QString toString(bool )const;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const;
    /**
     * @brief setHelpPath
     * @param path
     */
    void setHelpPath(QString path);
    /**
     * @brief getCopy
     * @return
     */
    virtual ExecutionNode* getCopy() const;
private:
    QString m_path;

};

#endif // HELPNODE_H
