/**
    QZmView - The Qt-based view-only ZoneMinder client
    Copyright (C) 2020 HexEdit, IFProject

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
**/

#ifndef GRIDLISTMODEL_H
#define GRIDLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QJsonObject>

class GridListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        UuidRole = Qt::UserRole + 1,
        NameRole,
        RunOnStartRole,
        ScreenRole,
        ColumnsRole,
        RowsRole,
        PreviewFpsRole,
        DisplayFpsRole,
        MonitorsRole,
    };

    explicit GridListModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool update(int index, const QVariantMap &values);
    Q_INVOKABLE bool insert(const QVariantMap &values);
    Q_INVOKABLE void remove(int index);

private:
    QList<QJsonObject> m_list;
};

#endif // GRIDLISTMODEL_H
