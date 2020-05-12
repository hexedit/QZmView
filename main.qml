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

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import Qt.labs.platform 1.1
import QZmView 1.0

ApplicationWindow {
    id: window
    width: 800
    height: 600
    title: "QZmView"
    visible: !Settings.runToTray
    Material.theme: Settings.darkTheme ? Material.Dark : Material.Light
    Material.primary: Settings.darkTheme ? Material.Purple : Material.Cyan
    Material.accent: Settings.darkTheme ? Material.Teal : Material.Purple

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MonitorListPage {
            gridModel: gridModel
        }
    }

    GridListModel {
        id: gridModel
    }

    SystemTrayIcon {
        id: tray
        visible: true
        iconSource: "qrc:/images/zmicon.png"
        tooltip: window.title

        menu: Menu {

            MenuItem {
                text: qsTr("Show")
                font.bold: true
                onTriggered: tray.showWindow()
            }

            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }

        onActivated: {
            if (reason !== SystemTrayIcon.Context) {
                showWindow();
            }
        }

        function showWindow() {
            window.show();
            window.raise();
            window.requestActivate();
        }
    }

    onClosing: {
        if (!Settings.closeToTray) {
            Qt.quit();
        }
    }
}
