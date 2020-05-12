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

#include "qzmsettings.h"

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>

// All settings read from / written to config file / registry directly
// Run on start setting based on startup file / registry entry

QZmSettings::QZmSettings(QObject *parent)
    : QObject(parent)
{
    m_settings = new QSettings(QSettings::UserScope,
                               QCoreApplication::organizationName(),
                               QCoreApplication::applicationName());
}

QZmSettings::~QZmSettings()
{
    Q_ASSERT(nullptr != m_settings);
    delete m_settings;
}

QZmSettings * QZmSettings::instance()
{
    static QZmSettings *instance = new QZmSettings();
    return instance;
}

const QString QZmSettings::zmBaseUrl() const
{
    Q_ASSERT(nullptr != m_settings);
    return m_settings->value(QStringLiteral("zm/base_url"), QStringLiteral("")).toString();
}

const QString QZmSettings::zmApiUrl() const
{
    return m_settings->value(QStringLiteral("zm/api_url"), QStringLiteral("")).toString();
}

const QString QZmSettings::zmCgiUrl() const
{
    return m_settings->value(QStringLiteral("zm/cgi_url"), QStringLiteral("")).toString();
}

bool QZmSettings::zmEnableAuth() const
{
    return m_settings->value(QStringLiteral("zm/enable_auth"), false).toBool();
}

bool QZmSettings::isZmAuth() const
{
    return m_settings->value(QStringLiteral("zm/zm_auth"), true).toBool();
}

const QString QZmSettings::authUsername() const
{
    return m_settings->value(QStringLiteral("zm/username"), QStringLiteral("")).toString();
}

const QString QZmSettings::authPassword() const
{
    return m_settings->value(QStringLiteral("zm/password"), QStringLiteral("")).toString();
}

bool QZmSettings::darkTheme() const
{
    return m_settings->value(QStringLiteral("ui/dark_theme"), false).toBool();
}

QVariant QZmSettings::autoStart() const
{
#if defined(Q_OS_UNIX)
    QDir autoStartDir(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation)
                      + QStringLiteral("/autostart"));
    QString filename(QString(QStringLiteral("%1.desktop")).arg(m_settings->applicationName()));
    return QFile(autoStartDir.absoluteFilePath(filename)).exists();
#elif defined(Q_OS_WIN32)
    QSettings autoStartReg(
                "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                QSettings::NativeFormat);
    return autoStartReg.contains(m_settings->applicationName());
#else
    return QVariant();
#endif
}

bool QZmSettings::runToTray() const
{
    return m_settings->value(QStringLiteral("ui/run_to_tray"), false).toBool();
}

bool QZmSettings::closeToTray() const
{
    return m_settings->value(QStringLiteral("ui/close_to_tray"), false).toBool();
}

bool QZmSettings::listMotion() const
{
    return m_settings->value(QStringLiteral("ui/list_motion"), false).toBool();
}

int QZmSettings::listMaxFps() const
{
    return m_settings->value(QStringLiteral("ui/list_max_fps"), 1).toInt();
}

void QZmSettings::setZmBaseUrl(const QString &zmBaseUrl)
{
    if (this->zmBaseUrl() != zmBaseUrl) {
        m_settings->setValue(QStringLiteral("zm/base_url"), zmBaseUrl);
        emit zmBaseUrlChanged();
        emit changed();
    }
}

void QZmSettings::setZmApiUrl(const QString &zmApiUrl)
{
    if (this->zmApiUrl() != zmApiUrl) {
        m_settings->setValue(QStringLiteral("zm/api_url"), zmApiUrl);
        emit zmApiUrlChanged();
        emit changed();
    }
}

void QZmSettings::setZmCgiUrl(const QString &zmCgiUrl)
{
    if (this->zmCgiUrl() != zmCgiUrl) {
        m_settings->setValue(QStringLiteral("zm/cgi_url"), zmCgiUrl);
        emit zmCgiUrlChanged();
        emit changed();
    }
}

void QZmSettings::setZmEnableAuth(bool zmEnableAuth)
{
    if (this->zmEnableAuth() != zmEnableAuth) {
        m_settings->setValue(QStringLiteral("zm/enable_auth"), zmEnableAuth);
        emit zmEnableAuthChanged();
        emit changed();
    }
}

void QZmSettings::setIsZmAuth(bool isZmAuth)
{
    if (this->isZmAuth() != isZmAuth) {
        m_settings->setValue(QStringLiteral("zm/zm_auth"), isZmAuth);
        emit isZmAuthChanged();
        emit changed();
    }
}

void QZmSettings::setAuthUsername(const QString &authUsername)
{
    if (this->authUsername() != authUsername) {
        m_settings->setValue(QStringLiteral("zm/username"), authUsername);
        emit authUsernameChanged();
        emit changed();
    }
}

void QZmSettings::setAuthPassword(const QString &authPassword)
{
    if (this->authPassword() != authPassword) {
        m_settings->setValue(QStringLiteral("zm/password"), authPassword);
        emit authPasswordChanged();
        emit changed();
    }
}

void QZmSettings::setDarkTheme(bool darkTheme)
{
    if (this->darkTheme() != darkTheme) {
        m_settings->setValue(QStringLiteral("ui/dark_theme"), darkTheme);
        emit darkThemeChanged();
        emit changed();
    }
}

void QZmSettings::setAutoStart(const QVariant &autoStart)
{
    QVariant current = this->autoStart();
    if (current.isNull())
        return;
    if (current == autoStart)
        return;

#if defined(Q_OS_UNIX)
    QDir autoStartDir(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation)
                      + QStringLiteral("/autostart"));
    QString filename(QString(QStringLiteral("%1.desktop")).arg(m_settings->applicationName()));
    QFile autoStartFile(autoStartDir.absoluteFilePath(filename));
    if (autoStart.toBool()) {
        if (!autoStartFile.open(QFile::WriteOnly))
            return;
        QString autoStartContent = QStringLiteral(
                    "[Desktop Entry]\n"
                    "Type=Application\n"
                    "Terminal=false\n"
                    "Name=%1\n"
                    "Exec=%2\n");
        autoStartFile.write(autoStartContent
                            .arg(QCoreApplication::applicationName())
                            .arg(QCoreApplication::applicationFilePath())
                            .toUtf8());
    }
    else {
        if (!autoStartFile.remove())
            return;
    }
    emit autoStartChanged();
#elif defined(Q_OS_WIN32)
    QSettings autoStartReg(
                "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                QSettings::NativeFormat);
    if (autoStart.toBool()) {
        autoStartReg.setValue(
                    m_settings->applicationName(),
                    QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
    }
    else {
        autoStartReg.remove(m_settings->applicationName());
    }
    autoStartReg.sync();
    emit autoStartChanged();
#endif
}

void QZmSettings::setRunToTray(bool runToTray)
{
    if (this->runToTray() != runToTray) {
        m_settings->setValue(QStringLiteral("ui/run_to_tray"), runToTray);
        // emit runToTrayChanged();
        // My, currently, only workaround to prevent window hide on saving settings
        emit changed();
    }
}

void QZmSettings::setCloseToTray(bool closeToTray)
{
    if (this->closeToTray() != closeToTray) {
        m_settings->setValue(QStringLiteral("ui/close_to_tray"), closeToTray);
        emit closeToTrayChanged();
        emit changed();
    }
}

void QZmSettings::setListMotion(bool listMotion)
{
    if (this->listMotion() != listMotion) {
        m_settings->setValue(QStringLiteral("ui/list_motion"), listMotion);
        emit listMotionChanged();
        emit changed();
    }
}

void QZmSettings::setListMaxFps(int listMaxFps)
{
    if (this->listMaxFps() != listMaxFps) {
        m_settings->setValue(QStringLiteral("ui/list_max_fps"), listMaxFps);
        emit listMaxFpsChanged();
        emit changed();
    }
}
