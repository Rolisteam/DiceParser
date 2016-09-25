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
#include "commandmodel.h"

CommandModel::CommandModel()
{

}

QVariant CommandModel::data(const QModelIndex &index, int role) const
{
    QPair<QString,QString> indexP = m_data.at(index.row());
    if(role == NameRole)
    {
        return indexP.first;
    }
    else if(role == CmdRole)
    {
        return indexP.second;
    }
}

int CommandModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}
QHash<int, QByteArray>  CommandModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[CmdRole] = "cmd";
    return roles;
}
void CommandModel::insertCmd(QString name, QString cmd)
{
    QModelIndex index;
    beginInsertRows(index,0,0);
    m_data.prepend(QPair<QString,QString>(name,cmd));
    endInsertRows();
}
