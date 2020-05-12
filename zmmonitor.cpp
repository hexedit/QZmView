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

#include "zmmonitor.h"

ZmMonitor::ZmMonitor(QObject *parent)
    : QObject(parent),
      m_id(0),
      m_width(0),
      m_height(0)
{
}

ZmMonitor::ZmMonitor(const ZmMonitor &copy)
{
    *this = copy;
}

ZmMonitor& ZmMonitor::operator =(const ZmMonitor &copy)
{
    m_id = copy.m_id;
    m_name = copy.m_name;
    m_width = copy.m_width;
    m_height = copy.m_height;
    return *this;
}

int ZmMonitor::id() const
{
    return m_id;
}

const QString ZmMonitor::name() const
{
    return m_name;
}

int ZmMonitor::width() const
{
    return m_width;
}

int ZmMonitor::height() const
{
    return m_height;
}

void ZmMonitor::setId(int id)
{
    m_id = id;
    emit idChanged();
}

void ZmMonitor::setName(const QString &name)
{
    m_name = name;
    emit nameChanged();
}

void ZmMonitor::setWidth(int width)
{
    m_width = width;
    emit widthChanged();
}

void ZmMonitor::setHeight(int height)
{
    m_height = height;
    emit heightChanged();
}

const ZmMonitor ZmMonitor::fromJson(const QJsonObject &json)
{
    ZmMonitor monitor;

    QJsonObject mobj = json.value(QStringLiteral("Monitor")).toObject();
    if (!mobj.isEmpty()) {
        monitor.m_id = mobj.value(QStringLiteral("Id")).toString().toInt();
        monitor.m_name = mobj.value(QStringLiteral("Name")).toString();
        monitor.m_width = mobj.value(QStringLiteral("Width")).toString().toInt();
        monitor.m_height = mobj.value(QStringLiteral("Height")).toString().toInt();
    }

    return monitor;
}
