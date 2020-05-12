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

#ifndef QZMMONITOR_H
#define QZMMONITOR_H

#include <QQuickPaintedItem>
#include <QNetworkAccessManager>
#include <QImage>

#include "qzmsettings.h"

class QZmMonitor : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(int monitorId READ monitorId WRITE setMonitorId NOTIFY idChanged)
    Q_PROPERTY(int maxfps READ maxfps WRITE setMaxfps NOTIFY maxfpsChanged)
    Q_PROPERTY(bool single READ single WRITE setSingle NOTIFY singleChanged)

    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged)

public:
    explicit QZmMonitor(QQuickItem *parent = nullptr);

    int monitorId() const;
    int maxfps() const;
    bool single() const;

    bool isRunning() const;

    void setMonitorId(int monitorId);
    void setMaxfps(int maxfps);
    void setSingle(bool single);

    virtual void paint(QPainter *painter);

private:
    int m_id;
    int m_maxfps;
    bool m_single;

    bool m_isRunning;

    QZmSettings *m_settings;
    QNetworkAccessManager *m_mgr;
    QByteArray m_rdBuf;
    QImage m_image;

    void setIsRunning(bool isRunning = true);

    bool checkReply(QNetworkReply *reply);

signals:
    void idChanged();
    void maxfpsChanged();
    void singleChanged();

    void isRunningChanged();

    void stopCurrent();
    void imageReady(const QByteArray &image);

public slots:
    void restart();

private slots:
    void singleFinished();
    void readyRead();
    void stopped();
    void onImageReady(const QByteArray &image);
    void onRunningChanged();
};

#endif // QZMMONITOR_H
