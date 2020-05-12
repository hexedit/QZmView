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

#include "zmtoken.h"

#include <QJsonDocument>
#include <QJsonObject>

ZmToken::ZmToken(QObject *parent) : QObject(parent)
{
}

void ZmToken::update(const QString &token)
{
    m_token = QString();
    m_type = QString();
    m_issuer = QString();
    m_expires = QDateTime();

    QStringList parts = token.split('.');
    if (parts.size() != 3)
        return;

    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromBase64(parts[1].toUtf8()));
    if (!doc.isObject())
        return;

    QJsonObject jtok = doc.object();
    if (!jtok.contains(QStringLiteral("iss")))
        return;
    if (!jtok.contains(QStringLiteral("exp")))
        return;
    if (!jtok.contains(QStringLiteral("type")))
        return;

    m_token = token;
    m_type = jtok.value(QStringLiteral("type")).toString();
    m_issuer = jtok.value(QStringLiteral("iss")).toString();
    m_expires = QDateTime::fromSecsSinceEpoch(jtok.value(QStringLiteral("exp")).toInt());
}

bool ZmToken::isValid() const
{
    if (m_issuer != QStringLiteral("ZoneMinder"))
        return false;
    if (m_expires < QDateTime::currentDateTime())
        return false;
    return true;
}

bool ZmToken::isAccess() const
{
    return m_type.compare(QStringLiteral("access"), Qt::CaseInsensitive) == 0;
}

bool ZmToken::isRefresh() const
{
    return m_type.compare(QStringLiteral("refresh"), Qt::CaseInsensitive) == 0;
}

const QString ZmToken::toString() const
{
    return m_token;
}
