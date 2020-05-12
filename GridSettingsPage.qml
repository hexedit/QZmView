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

import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.2
import QZmView 1.0
import "IconFont.js" as IconFont

Page {
    id: root

    property GridListModel gridModel

    header: ToolBar {
        ToolButton {
            font.family: IconFont.name
            font.pixelSize: 24
            text: IconFont.icons.arrow_back
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            onClicked: root.StackView.view.pop()
        }

        Label {
            text: qsTr("Grids")
            font.pixelSize: 20
            anchors.centerIn: parent
        }

        ToolButton {
            id: addButton
            font.family: IconFont.name
            font.pixelSize: 24
            text: IconFont.icons.add
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                var page = root.StackView.view.push("GridEditPage.qml", {});
                page.saving.connect(function() {
                    gridsList.model.insert({
                                               name: page.name,
                                               runOnStart: page.runOnStart,
                                               screen: page.screen,
                                               columns: page.columns,
                                               rows: page.rows,
                                               previewFps: page.previewFps,
                                               displayFps: page.displayFps,
                                               monitors: page.monitors,
                                           });
                });
            }
        }

        ToolButton {
            id: refreshButton
            font.family: IconFont.name
            font.pixelSize: 24
            text: IconFont.icons.refresh
            anchors.right: addButton.left
            anchors.verticalCenter: parent.verticalCenter
            onClicked: gridsList.model.refresh()
        }
    }

    ListView {
        id: gridsList
        anchors {
            fill: parent
            margins: 0
        }
        model: gridModel
        delegate: ItemDelegate {
            width: ListView.view.width
            onClicked: {
                var page = root.StackView.view.push("GridEditPage.qml", {
                                                        name: name,
                                                        runOnStart: runOnStart,
                                                        screen: screen,
                                                        columns: columns,
                                                        rows: rows,
                                                        previewFps: previewFps,
                                                        displayFps: displayFps,
                                                        monitors: monitors,
                                                    })
                page.saving.connect(function() {
                    ListView.view.model.update(index, {
                                                   name: page.name,
                                                   runOnStart: page.runOnStart,
                                                   screen: page.screen,
                                                   columns: page.columns,
                                                   rows: page.rows,
                                                   previewFps: page.previewFps,
                                                   displayFps: page.displayFps,
                                                   monitors: page.monitors,
                                               });
                });
            }

            RowLayout {
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10

                Label {
                    Layout.leftMargin: 10
                    font.family: IconFont.name
                    font.pixelSize: 24
                    text: IconFont.icons.grid_on
                }

                Label {
                    font.bold: true
                    text: name
                }

                Label {
                    text: `${columns}x${rows}`
                }
            }

            ToolButton {
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                font {
                    family: IconFont.name
                    pixelSize: 24
                }
                text: IconFont.icons.delete
                onClicked: confirmDelete.open()

                MessageDialog {
                    id: confirmDelete
                    title: qsTr("Delete grid")
                    text: qsTr(`Delete grid ${name} ${columns}x${rows}?`)
                    standardButtons: StandardButton.Yes | StandardButton.No
                    onYes: gridsList.model.remove(index)
                }
            }
        }
        ScrollBar.vertical: ScrollBar {}
    }

}
