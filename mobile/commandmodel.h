/***************************************************************************
 *   Copyright (C) 2016 by Renaud Guezennec                                *
 *   http://www.rolisteam.org/contact                                      *
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
#ifndef COMMANDMODEL_H
#define COMMANDMODEL_H

#include <QObject>

#include <QAbstractListModel>

class CommandModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CustomRole
    {
        NameRole= Qt::UserRole + 1,
        CmdRole
    };
    CommandModel();

    virtual QVariant data(const QModelIndex& index, int role= Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent) const;

    QHash<int, QByteArray> roleNames() const;

public slots:
    void insertCmd(QString name, QString cmd);

private:
    QList<QPair<QString, QString>> m_data;
};

#endif // COMMANDMODEL_H
