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

#ifndef MONITORLISTMODEL_H
#define MONITORLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "zmmonitor.h"

class MonitorListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MonitorListModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void refresh();

private:
    void doRefresh();

    QList<ZmMonitor> m_list;

private slots:
    void loggedIn();
    void listFinished();
};

#endif // MONITORLISTMODEL_H
