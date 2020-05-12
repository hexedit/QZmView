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

#include "monitorlistmodel.h"
#include "zmapi.h"

#include <QJsonDocument>
#include <QJsonArray>

enum Roles {
    IdRole = Qt::UserRole + 1,
    NameRole,
    WidthRole,
    HeightRole,
};

MonitorListModel::MonitorListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QObject::connect(ZmApi::instance(), SIGNAL(loggedIn()), this, SLOT(loggedIn()));
    refresh();
}

int MonitorListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_list.size();
}

QVariant MonitorListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ZmMonitor obj = m_list[index.row()];

    switch (role) {
    case IdRole:
        return obj.id();
    case NameRole:
        return obj.name();
    case WidthRole:
        return obj.width();
    case HeightRole:
        return obj.height();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MonitorListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[WidthRole] = "width";
    roles[HeightRole] = "height";
    return roles;
}

void MonitorListModel::refresh()
{
    if (ZmApi::instance()->isLoggedIn())
        doRefresh();
    else
        ZmApi::instance()->login();
}

void MonitorListModel::doRefresh()
{
    QNetworkReply *reply = ZmApi::instance()->listMonitors();
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(listFinished()));
}

void MonitorListModel::loggedIn()
{
    doRefresh();
}

void MonitorListModel::listFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() != QNetworkReply::NoError)
        return;

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if (!doc.isObject())
        return;

    QJsonValue list = doc.object().value(QStringLiteral("monitors"));
    if (!list.isArray())
        return;

    beginRemoveRows(QModelIndex(), 0, m_list.size() - 1);
    m_list.clear();
    endRemoveRows();

    foreach (const QJsonValue &item, list.toArray()) {
        if (!item.isObject())
            continue;
        m_list.append(ZmMonitor::fromJson(item.toObject()));
    }

    beginInsertRows(QModelIndex(), 0, m_list.size() - 1);
    endInsertRows();
}
