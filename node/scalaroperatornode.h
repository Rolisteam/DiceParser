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
#ifndef SCALAROPERATORNODE_H
#define SCALAROPERATORNODE_H

#include <QMap>
#include <QChar>

#include "executionnode.h"
#include "result/scalarresult.h"


/**
 * @brief The ScalarOperatorNode class
 */
class ScalarOperatorNode : public ExecutionNode
{
public:
    enum ScalarOperator {PLUS,MINUS,DIVIDE,MULTIPLICATION};
    ScalarOperatorNode();
	virtual ~ScalarOperatorNode();
    virtual void run(ExecutionNode*);
    bool setOperatorChar(QChar c);
    void setInternalNode(ExecutionNode* node);

    virtual QString toString()const;
    virtual qint64 getPriority() const;

    void generateDotTree(QString& s);
    /**
     * @brief getErrorList
     * @return
     */
    virtual QMap<ExecutionNode::ERROR_CODE,QString> getExecutionErrorMap();

private:
    qint64 add(qint64,qint64);
    qint64 substract(qint64,qint64);
    qreal divide(qint64,qint64);
    qint64 multiple(qint64,qint64);

private:
	ScalarOperator m_operator;
    ExecutionNode* m_internalNode;
    QMap<QChar,ScalarOperator> m_scalarOperationList;
    ScalarResult* m_scalarResult;
};

#endif // SCALAROPERATORNODE_H

