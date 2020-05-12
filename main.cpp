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

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QFontDatabase>
#include <QtQml>

#include <qzmsettings.h>
#include <zmapi.h>
#include <monitorlistmodel.h>
#include <gridlistmodel.h>
#include <qzmmonitor.h>

int main(int argc, char *argv[])
{
    // Setting core application parameters
    QApplication::setOrganizationName(QStringLiteral("IFProject"));
    QApplication::setApplicationName(QStringLiteral("QZmView"));
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickStyle::setStyle(QStringLiteral("Material"));

    // Initializing application
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QStringLiteral(":/images/zmicon.png")));
    QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/MaterialIcons-Regular.ttf"));
    app.setQuitOnLastWindowClosed(false);

    // Registering QML types
    qmlRegisterSingletonType<QZmSettings>("QZmView", 1, 0, "Settings",
                                          [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);
        return QZmSettings::instance();
    });
    qmlRegisterSingletonType<ZmApi>("QZmView", 1, 0, "Api",
                                    [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);
        return ZmApi::instance();
    });
    qmlRegisterType<MonitorListModel>("QZmView", 1, 0, "MonitorListModel");
    qmlRegisterType<GridListModel>("QZmView", 1, 0, "GridListModel");
    qmlRegisterType<QZmMonitor>("QZmView", 1, 0, "Monitor");

    // Running main application window
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // Scanning (and running) startup-enabled grids
    {
        GridListModel gridModel;
        for (int gx = 0; gx < gridModel.rowCount(QModelIndex()); gx++) {
            QModelIndex index(gridModel.index(gx));
            if (gridModel.data(index, GridListModel::RunOnStartRole).toBool()) {
                QQmlComponent cGrid(&engine);
                cGrid.loadUrl(QStringLiteral("qrc:/GridWindow.qml"));
                if (!cGrid.isReady())
                    continue;
                QObject *grid = cGrid.create();
                grid->setProperty("screenIndex", gridModel.data(index, GridListModel::ScreenRole));
                grid->setProperty("columns", gridModel.data(index, GridListModel::ColumnsRole));
                grid->setProperty("rows", gridModel.data(index, GridListModel::RowsRole));
                grid->setProperty("previewFps", gridModel.data(index, GridListModel::PreviewFpsRole));
                grid->setProperty("displayFps", gridModel.data(index, GridListModel::DisplayFpsRole));
                grid->setProperty("monitors", gridModel.data(index, GridListModel::MonitorsRole));
                QMetaObject::invokeMethod(grid, "show");
            }
        }
    }

    return app.exec();
}
