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

#include "zmapi.h"
#include "qzmsettings.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

ZmApi::ZmApi(QObject *parent) : QObject(parent)
{
    QObject::connect(QZmSettings::instance(), SIGNAL(zmApiUrlChanged()),
                     this, SLOT(login()));
}

ZmApi* ZmApi::instance()
{
    static ZmApi *instance = new ZmApi();
    return instance;
}

const QString ZmApi::version() const
{
    return m_version;
}

const QString ZmApi::zmVersion() const
{
    return m_zmVersion;
}

bool ZmApi::isLoggedIn() const
{
    return m_refreshToken.isValid();
}

QNetworkReply* ZmApi::listMonitors()
{
    QUrl url = makeUrl(QStringLiteral("/monitors"));
    return m_mgr.get(QNetworkRequest(url));
}

void ZmApi::setVersion(const QString &version)
{
    m_version = version;
}

void ZmApi::setZmVersion(const QString &zmVersion)
{
    m_zmVersion = zmVersion;
}

const QUrl ZmApi::makeUrl(const QString &path, bool auth) const
{
    QUrl url(QZmSettings::instance()->zmApiUrl() + path + QStringLiteral(".json"));
    if (auth && m_accessToken.isValid()) {
        QUrlQuery params;
        params.addQueryItem(QStringLiteral("token"), m_accessToken.toString());
        url.setQuery(params);
    }
    return url;
}

void ZmApi::login()
{
    QUrl url = makeUrl(QStringLiteral("/host/login"), false);
    QHttpMultiPart *data = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart username;
    username.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QStringLiteral("form-data; name=\"user\""));
    username.setBody(QZmSettings::instance()->authUsername().toUtf8());
    data->append(username);

    QHttpPart password;
    password.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QStringLiteral("form-data; name=\"pass\""));
    password.setBody(QZmSettings::instance()->authPassword().toUtf8());
    data->append(password);

    QNetworkReply *reply = m_mgr.post(QNetworkRequest(url), data);
    data->setParent(reply);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(loginFinished()));
}

void ZmApi::loginFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() != QNetworkReply::NoError)
        return;

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if (!doc.isObject())
        return;

    QJsonObject auth = doc.object();
    m_accessToken = auth.value(QStringLiteral("access_token")).toString();
    int accessTtl = auth.value(QStringLiteral("access_token_expires")).toInt() * 1000;
    m_refreshToken = auth.value(QStringLiteral("refresh_token")).toString();
    int refreshTtl = auth.value(QStringLiteral("refersh_token_expires")).toInt() * 1000;

    if (accessTtl > 0)
        QTimer::singleShot(accessTtl, this, SLOT(refreshToken()));
    if (refreshTtl > 0)
        QTimer::singleShot(refreshTtl, this, SLOT(login()));

    emit loggedIn();

    QString version = auth.value(QStringLiteral("apiversion")).toString();
    if (version != m_version) {
        m_version = version;
        emit versionChanged();
    }

    QString zmVersion = auth.value(QStringLiteral("version")).toString();
    if (zmVersion != m_zmVersion) {
        m_zmVersion = zmVersion;
        emit zmVersionChanged();
    }
}

void ZmApi::refreshToken()
{
    QUrl url = makeUrl(QStringLiteral("/host/login"), false);

    QUrlQuery params;
    params.addQueryItem(QStringLiteral("token"), m_refreshToken.toString());
    url.setQuery(params);

    QNetworkReply *reply = m_mgr.get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(loginFinished()));
}
