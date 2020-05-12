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

#ifndef ZMTOKEN_H
#define ZMTOKEN_H

#include <QObject>
#include <QDateTime>

class ZmToken : public QObject
{
    Q_OBJECT

public:
    explicit ZmToken(QObject *parent = nullptr);
    inline void operator =(const QString &token) {
        update(token);
    }

    void update(const QString &token);
    bool isValid() const;
    bool isAccess() const;
    bool isRefresh() const;
    const QString toString() const;

private:
    QString m_token;
    QString m_type;
    QString m_issuer;
    QDateTime m_expires;
};

#endif // ZMTOKEN_H
