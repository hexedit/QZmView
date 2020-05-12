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

#include "qzmmonitor.h"
#include "qzmsettings.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPainter>

QZmMonitor::QZmMonitor(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_id(0),
      m_maxfps(0),
      m_single(false),
      m_isRunning(false),
      m_settings(QZmSettings::instance()),
      m_mgr(new QNetworkAccessManager(this)),
      m_image()
{
    // Restart stream if parameters changed
    connect(this, SIGNAL(idChanged()), this, SLOT(restart()));
    connect(this, SIGNAL(maxfpsChanged()), this, SLOT(restart()));
    connect(this, SIGNAL(singleChanged()), this, SLOT(restart()));
    connect(m_settings, SIGNAL(zmCgiUrlChanged()), this, SLOT(restart()));
    connect(m_settings, SIGNAL(zmEnableAuthChanged()), this, SLOT(restart()));
    connect(m_settings, SIGNAL(authUsernameChanged()), this, SLOT(restart()));
    connect(m_settings, SIGNAL(authPasswordChanged()), this, SLOT(restart()));
    connect(m_settings, SIGNAL(isZmAuthChanged()), this, SLOT(restart()));

    connect(this, SIGNAL(stopCurrent()), this, SLOT(stopped()));
    connect(this, SIGNAL(imageReady(QByteArray)), this, SLOT(onImageReady(QByteArray)));
    connect(this, SIGNAL(isRunningChanged()), this, SLOT(onRunningChanged()));
}

int QZmMonitor::monitorId() const
{
    return m_id;
}

int QZmMonitor::maxfps() const
{
    return m_maxfps;
}

bool QZmMonitor::single() const
{
    return m_single;
}

bool QZmMonitor::isRunning() const
{
    return m_isRunning;
}

void QZmMonitor::setMonitorId(int id)
{
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

void QZmMonitor::setMaxfps(int maxfps)
{
    if (m_maxfps != maxfps) {
        m_maxfps = maxfps;
        if (!m_single)
            emit maxfpsChanged();
    }
}

void QZmMonitor::setSingle(bool single)
{
    if (m_single != single) {
        m_single = single;
        emit singleChanged();
    }
}

void QZmMonitor::paint(QPainter *painter)
{
    QImage image(m_image);
    if (image.isNull())
        image.load(QStringLiteral(":/images/zmicon.png"));
    painter->drawImage(0, 0, image.scaled(width(), height()));
}

void QZmMonitor::setIsRunning(bool isRunning)
{
    if (m_isRunning != isRunning) {
        m_isRunning = isRunning;
        emit isRunningChanged();
    }
}

void QZmMonitor::restart()
{
    emit stopCurrent();
    if (m_id < 1)
        return;

    QUrl url(m_settings->zmCgiUrl() + QStringLiteral("/nph-zms"));
    QUrlQuery params;
    params.addQueryItem(QStringLiteral("monitor"), QString::number(m_id));
    if (m_settings->zmEnableAuth() && m_settings->isZmAuth()) {
        params.addQueryItem(QStringLiteral("user"), m_settings->authUsername());
        params.addQueryItem(QStringLiteral("pass"), m_settings->authPassword());
    }
    if (m_single) {
        params.addQueryItem(QStringLiteral("mode"), QStringLiteral("single"));
    }
    else if (m_maxfps > 0) {
        params.addQueryItem(QStringLiteral("maxfps"), QString::number(m_maxfps));
    }
    url.setQuery(params);

    QNetworkReply *reply = m_mgr->get(QNetworkRequest(url));
    if (m_single) {
        connect(reply, SIGNAL(finished()), this, SLOT(singleFinished()));
    }
    else {
        connect(reply, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(this, SIGNAL(stopCurrent()), reply, SLOT(abort()));
        setIsRunning();
    }
}

void QZmMonitor::singleFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() != QNetworkReply::NoError)
        return;
    emit imageReady(reply->readAll());
}

bool QZmMonitor::checkReply(QNetworkReply *reply)
{
    // check for error
    if (reply->error() != QNetworkReply::NoError)
        return false;

    // check for supported container content type
    QStringList contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString().split(';');
    if (contentType.size() < 2)
        return false;
    if (contentType.at(0).compare(QLatin1String("multipart/x-mixed-replace"), Qt::CaseInsensitive) != 0)
        return false;
    return true;
}

void QZmMonitor::readyRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!checkReply(reply)) {
        emit stopCurrent();
        return;
    }

    // Extracting images using JPEG start/end patterns
    m_rdBuf += reply->readAll();
    int start = m_rdBuf.indexOf(QByteArrayLiteral("\xff\xd8\xff"));
    int end = m_rdBuf.lastIndexOf(QByteArrayLiteral("\xff\xd9"));
    // Checking if we have whole JPEG in read buffer
    if ((start != -1) && (end != -1)) {
        emit imageReady(m_rdBuf.mid(start, end - start + 2));
        m_rdBuf = m_rdBuf.mid(end + 2);
    }
}

void QZmMonitor::stopped()
{
    setIsRunning(false);
    m_rdBuf.clear();
}

void QZmMonitor::onImageReady(const QByteArray &image)
{
    m_image = QImage::fromData(image);
    update();
}

void QZmMonitor::onRunningChanged()
{
    if (!m_isRunning)
        emit imageReady(QByteArray());
}
