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
import QZmView 1.0
import "IconFont.js" as IconFont

Page {
    id: root

    property GridListModel gridModel

    header: ToolBar {
        Label {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 16
            text: Api.zmVersion
        }

        Label {
            text: qsTr("Monitors")
            font.pixelSize: 20
            anchors.centerIn: parent
        }

        ToolButton {
            id: settingsButton
            font.family: IconFont.name
            font.pixelSize: 24
            text: IconFont.icons.settings
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            onClicked: root.StackView.view.push("SettingsPage.qml", {})
        }

        ToolButton {
            id: refreshButton
            font.family: IconFont.name
            font.pixelSize: 24
            text: IconFont.icons.refresh
            anchors.right: settingsButton.left
            anchors.verticalCenter: parent.verticalCenter
            onClicked: monitorList.model.refresh()
        }

        ToolButton {
            id: gridsButton
            font.family: IconFont.name
            font.pixelSize: 24
            text: IconFont.icons.grid_on
            anchors.right: refreshButton.left
            anchors.verticalCenter: parent.verticalCenter
            onClicked: gridsMenu.popup()

            Menu {
                id: gridsMenu

                Menu {
                    title: qsTr("Grids")
                    enabled: count > 0

                    Repeater {
                        model: gridModel

                        MenuItem {
                            text: name
                            onTriggered: {
                                var component = Qt.createComponent("GridWindow.qml");
                                if (component.status === Component.Ready) {
                                    var params = {
                                        screenIndex: model.screen,
                                        columns: columns,
                                        rows: rows,
                                        previewFps: previewFps,
                                        displayFps: displayFps,
                                        monitors: monitors,
                                    };
                                    var grid = component.createObject(parent, params);
                                    grid.show();
                                }
                            }
                        }
                    }
                }

                MenuSeparator {}

                MenuItem {
                    text: qsTr("Settings")
                    onTriggered: root.StackView.view.push("GridSettingsPage.qml", {
                                                              gridModel: gridModel
                                                          })
                }
            }
        }
    }

    GridView {
        property int row: Math.floor(Math.sqrt(count) + 0.9)

        id: monitorList
        anchors.fill: parent
        anchors.margins: 2
        cellWidth: width / row
        cellHeight: height / Math.floor((count + (row - 1)) / row)
        model: MonitorListModel {}
        delegate: MonitorDelegate {
            width: GridView.view.cellWidth
            height: GridView.view.cellHeight
            monitorId: model.id
            name: model.name
            single: !Settings.listMotion
            maxfps: Settings.listMaxFps
            onDoubleClicked: {
                root.StackView.view.push("MonitorDetailPage.qml", {
                                             monitorId: model.id,
                                             monitorName: model.name
                                         })
            }
        }
        interactive: false
    }
}
