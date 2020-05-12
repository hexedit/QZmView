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

#include "gridlistmodel.h"

#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QUuid>
#include <QDebug>

static const QString extension(QStringLiteral(".grid.json"));

GridListModel::GridListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    refresh();
}

int GridListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_list.size();
}

QVariant GridListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    return m_list.at(index.row()).value(roleNames().value(role)).toVariant();
}

QHash<int, QByteArray> GridListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UuidRole] = "uuid";
    roles[NameRole] = "name";
    roles[RunOnStartRole] = "runOnStart";
    roles[ScreenRole] = "screen";
    roles[ColumnsRole] = "columns";
    roles[RowsRole] = "rows";
    roles[PreviewFpsRole] = "previewFps";
    roles[DisplayFpsRole] = "displayFps";
    roles[MonitorsRole] = "monitors";
    return roles;
}

Qt::ItemFlags GridListModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable;
}

void GridListModel::refresh()
{
    beginRemoveRows(QModelIndex(), 0, m_list.size() - 1);
    m_list.clear();
    endRemoveRows();

    // Scan .grid.json files in application data folder
    QDir appData(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QStringList files = appData.entryList({QStringLiteral("*") + extension}, QDir::Files, QDir::Time);
    foreach (QString file, files) {
        QFile gridFile(appData.absoluteFilePath(file));
        if (!gridFile.open(QFile::ReadOnly))
            continue;
        QJsonDocument doc = QJsonDocument::fromJson(gridFile.readAll());
        if (!doc.isObject())
            continue;
        QJsonObject grid = doc.object();
        QVariant uuid(file.section('.', 0, 0));
        grid.insert(QStringLiteral("uuid"), QJsonValue::fromVariant(uuid));
        m_list.append(grid);
    }

    beginInsertRows(QModelIndex(), 0, m_list.size() - 1);
    endInsertRows();
}

bool GridListModel::update(int index, const QVariantMap &values)
{
    if (index >= m_list.size())
        return false;

    QJsonObject grid = m_list[index];
    QString uuidRole = roleNames().value(UuidRole);
    QString uuid = grid.value(uuidRole).toString();

    // Creating data folder if not exists
    QDir appData(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QFile gridFile(appData.absoluteFilePath(uuid + extension));
    if (!gridFile.open(QFile::WriteOnly))
        return false;

    // Update data for roles in `values`
    QVector<int> changedRoles;
    QHash<int, QByteArray> roles = roleNames();
    for (QHash<int, QByteArray>::const_iterator ri = roles.begin(); ri != roles.end(); ri++) {
        QString role = QString::fromUtf8(ri.value());
        if (values.contains(role) && (grid[role].toVariant() != values.value(role))) {
            grid[role] = QJsonValue::fromVariant(values.value(role));
            changedRoles.append(ri.key());
        }
    }
    m_list[index] = grid;
    emit dataChanged(this->index(index), this->index(index), changedRoles);

    grid.remove(uuidRole);
    gridFile.write(QJsonDocument(grid).toJson());
    return true;
}

bool GridListModel::insert(const QVariantMap &values)
{
    QDir appData(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!appData.exists()) {
        if (!appData.mkpath(QStringLiteral(".")))
            return false;
    }

    QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QFile gridFile(appData.absoluteFilePath(uuid + extension));

    QHash<int, QByteArray> roles = roleNames();
    QString uuidRole = QString::fromUtf8(roles.value(UuidRole));
    roles.remove(UuidRole);

    // Setting data for roles from `values`
    // All roles except UUID are mandatory
    QJsonObject grid;
    foreach (QByteArray roleName, roles) {
        QString role = QString::fromUtf8(roleName);
        if (!values.contains(role))
            return false;
        grid[role] = QJsonValue::fromVariant(values.value(role));
    }

    if (!gridFile.open(QFile::WriteOnly))
        return false;
    gridFile.write(QJsonDocument(grid).toJson());

    beginInsertRows(QModelIndex(), m_list.size(), m_list.size());
    grid[uuidRole] = QJsonValue::fromVariant(uuid);
    m_list.append(grid);
    endInsertRows();

    return true;
}

void GridListModel::remove(int index)
{
    if (index >= m_list.size())
        return;

    QJsonObject grid = m_list[index];
    QString uuidRole = roleNames().value(UuidRole);
    QString uuid = grid.value(uuidRole).toString();

    QDir appData(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QFile gridFile(appData.absoluteFilePath(uuid + extension));
    if (!gridFile.remove())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_list.removeAt(index);
    endRemoveRows();
}
