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

#ifndef ZMMONITOR_H
#define ZMMONITOR_H

#include <QObject>
#include <QJsonObject>

class ZmMonitor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)

public:
    explicit ZmMonitor(QObject *parent = nullptr);
    ZmMonitor(const ZmMonitor &copy);
    ZmMonitor& operator =(const ZmMonitor &copy);

    int id() const;
    const QString name() const;
    int width() const;
    int height() const;

    void setId(int id);
    void setName(const QString &name);
    void setWidth(int width);
    void setHeight(int height);

    static const ZmMonitor fromJson(const QJsonObject &json);

private:
    int m_id;
    QString m_name;
    int m_width;
    int m_height;

signals:
    void idChanged();
    void nameChanged();
    void widthChanged();
    void heightChanged();
};

#endif // ZMMONITOR_H
