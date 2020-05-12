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

#ifndef ZMAPI_H
#define ZMAPI_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "zmtoken.h"

class ZmApi : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ZmApi)

    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(QString zmVersion READ zmVersion WRITE setZmVersion NOTIFY zmVersionChanged)

public:
    static ZmApi* instance();

    const QString version() const;
    const QString zmVersion() const;

    bool isLoggedIn() const;
    QNetworkReply* listMonitors();

private:
    explicit ZmApi(QObject *parent = nullptr);

    void setVersion(const QString &version);
    void setZmVersion(const QString &zmVersion);

    const QUrl makeUrl(const QString &path, bool auth = true) const;

    QNetworkAccessManager m_mgr;
    ZmToken m_accessToken;
    ZmToken m_refreshToken;

    QString m_version;
    QString m_zmVersion;

public slots:
    void login();

private slots:
    void loginFinished();
    void refreshToken();

signals:
    void loggedIn();
    void versionChanged();
    void zmVersionChanged();
};

#endif // ZMAPI_H
