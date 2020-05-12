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
import QZmView 1.0

ApplicationWindow {
    id: root
    screen: Qt.application.screens[screenIndex]
    width: screen.width
    height: screen.height
    flags: Qt.FramelessWindowHint
    Material.theme: Settings.darkTheme ? Material.Dark : Material.Light
    Material.primary: Settings.darkTheme ? Material.Purple : Material.Cyan
    Material.accent: Settings.darkTheme ? Material.Teal : Material.Purple

    property int screenIndex: 0
    property int columns: 2
    property int rows: 2
    property int previewFps: 0
    property int displayFps: 0
    property variant monitors: []

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: grid

        Page {
            id: grid

            GridView {
                anchors.fill: parent
                cellWidth: width / columns
                cellHeight: height / rows
                model: monitors
                delegate: MonitorDelegate {
                    width: GridView.view.cellWidth
                    height: GridView.view.cellHeight
                    monitorId: modelData
                    single: false
                    maxfps: previewFps

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        onReleased: {
                            if (mouse.button === Qt.RightButton) {
                                gridMenu.popup();
                            }
                        }

                        onDoubleClicked: {
                            if (mouse.button === Qt.LeftButton) {
                                monitor.monitorId = modelData;
                                stack.push(monitor);
                            }
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onReleased: {
                        gridMenu.popup();
                    }
                }
            }
        }

        Page {
            id: monitor
            visible: false

            property int monitorId: 0

            Monitor {
                anchors.fill: parent
                single: false
                maxfps: displayFps
                monitorId: monitor.monitorId
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onReleased: {
                    if (mouse.button === Qt.RightButton) {
                        gridMenu.popup();
                    }
                }
                onDoubleClicked: {
                    if (mouse.button === Qt.LeftButton) {
                        stack.pop();
                    }
                }
            }
        }

    }

    Menu {
        id: gridMenu

        MenuItem {
            text: qsTr("Close grid")
            onTriggered: root.close()
        }
    }
}
