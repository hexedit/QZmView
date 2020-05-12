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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class QZmSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString zmBaseUrl READ zmBaseUrl WRITE setZmBaseUrl NOTIFY zmBaseUrlChanged)
    Q_PROPERTY(QString zmApiUrl READ zmApiUrl WRITE setZmApiUrl NOTIFY zmApiUrlChanged)
    Q_PROPERTY(QString zmCgiUrl READ zmCgiUrl WRITE setZmCgiUrl NOTIFY zmCgiUrlChanged)
    Q_PROPERTY(bool zmEnableAuth READ zmEnableAuth WRITE setZmEnableAuth NOTIFY zmEnableAuthChanged)
    Q_PROPERTY(bool isZmAuth READ isZmAuth WRITE setIsZmAuth NOTIFY isZmAuthChanged)
    Q_PROPERTY(QString authUsername READ authUsername WRITE setAuthUsername NOTIFY authUsernameChanged)
    Q_PROPERTY(QString authPassword READ authPassword WRITE setAuthPassword NOTIFY authPasswordChanged)

    Q_PROPERTY(bool darkTheme READ darkTheme WRITE setDarkTheme NOTIFY darkThemeChanged)
    Q_PROPERTY(QVariant autoStart READ autoStart WRITE setAutoStart NOTIFY autoStartChanged)
    Q_PROPERTY(bool runToTray READ runToTray WRITE setRunToTray NOTIFY runToTrayChanged)
    Q_PROPERTY(bool closeToTray READ closeToTray WRITE setCloseToTray NOTIFY closeToTrayChanged)
    Q_PROPERTY(bool listMotion READ listMotion WRITE setListMotion NOTIFY listMotionChanged)
    Q_PROPERTY(int listMaxFps READ listMaxFps WRITE setListMaxFps NOTIFY listMaxFpsChanged)

private:
    Q_DISABLE_COPY(QZmSettings)
    QZmSettings(QObject *parent = nullptr);
    ~QZmSettings();

public:
    static QZmSettings * instance();

    //! Get ZM settings
    const QString zmBaseUrl() const;
    const QString zmApiUrl() const;
    const QString zmCgiUrl() const;
    bool zmEnableAuth() const;
    bool isZmAuth() const;
    const QString authUsername() const;
    const QString authPassword() const;

    //! Get UI settings
    bool darkTheme() const;
    QVariant autoStart() const;
    bool runToTray() const;
    bool closeToTray() const;
    bool listMotion() const;
    int listMaxFps() const;

    //! Set ZM settings
    void setZmBaseUrl(const QString &zmBaseUrl);
    void setZmApiUrl(const QString &zmApiUrl);
    void setZmCgiUrl(const QString &zmCgiUrl);
    void setZmEnableAuth(bool zmEnableAuth);
    void setIsZmAuth(bool isZmAuth);
    void setAuthUsername(const QString &authUsername);
    void setAuthPassword(const QString &authPassword);

    //! Set UI settings
    void setDarkTheme(bool darkTheme);
    void setAutoStart(const QVariant &autoStart);
    void setRunToTray(bool runToTray);
    void setCloseToTray(bool closeToTray);
    void setListMotion(bool listMotion);
    void setListMaxFps(int listMaxFps);

signals:
    void changed();

    // ZM settings change notifications
    void zmBaseUrlChanged();
    void zmApiUrlChanged();
    void zmCgiUrlChanged();
    void zmEnableAuthChanged();
    void isZmAuthChanged();
    void authUsernameChanged();
    void authPasswordChanged();

    // UI settings change notifications
    void darkThemeChanged();
    void autoStartChanged();
    void runToTrayChanged();
    void closeToTrayChanged();
    void listMotionChanged();
    void listMaxFpsChanged();

private:
    QSettings *m_settings;
};

#endif // SETTINGS_H
