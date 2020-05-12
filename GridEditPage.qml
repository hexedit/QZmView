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
import QZmView 1.0
import "IconFont.js" as IconFont

Page {
    id: root

    property string name: qsTr("New grid")
    property bool runOnStart: false
    property int screen: 0
    property int columns: 2
    property int rows: 2
    property int previewFps: 10
    property int displayFps: 0
    property variant monitors: []

    signal saving

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
            text: qsTr(`Grid: ${name}`)
            font.pixelSize: 20
            anchors.centerIn: parent
        }

        ToolButton {
            font.family: IconFont.name
            font.pixelSize: 24
            text: IconFont.icons.save
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                root.saving();
                root.StackView.view.pop();
            }
        }
    }

    ScrollView {
        id: scrollView
        anchors{
            fill: parent
            margins: 10
        }

        ColumnLayout {
            width: scrollView.width

            Label {
                text: qsTr("Name")
            }

            TextField {
                Layout.fillWidth: true
                placeholderText: qsTr("Grid name")
                selectByMouse: true
                text: name
                onTextChanged: name = text
            }

            CheckBox {
                text: qsTr("Run on program start")
                checked: runOnStart
                onCheckedChanged: runOnStart = checked
            }

            Label {
                text: qsTr("Run on display")
            }

            ComboBox {
                Layout.fillWidth: true
                model: Qt.application.screens
                textRole: "name"
                currentIndex: screen
                onActivated: screen = index
            }

            GroupBox {
                title: qsTr("Grid")
                Layout.fillWidth: true

                RowLayout {
                    spacing: 10

                    TextField {
                        placeholderText: columns
                        validator: RegExpValidator {
                            regExp: /^[0-9]{1,2}$/
                        }
                        horizontalAlignment: TextInput.AlignHCenter
                        selectByMouse: true
                        text: columns
                        onTextChanged: {
                            if (acceptableInput) {
                                columns = text;
                            }
                        }
                    }

                    Label {
                        text: "x"
                    }

                    TextField {
                        placeholderText: rows
                        validator: RegExpValidator {
                            regExp: /^[0-9]{1,2}$/
                        }
                        horizontalAlignment: TextInput.AlignHCenter
                        selectByMouse: true
                        text: rows
                        onTextChanged: {
                            if (acceptableInput) {
                                rows = text;
                            }
                        }
                    }
                }
            }

            GroupBox {
                title: qsTr("Maximum FPS")
                Layout.fillWidth: true

                RowLayout {
                    spacing: 10

                    Label {
                        text: qsTr("Preview")
                    }

                    TextField {
                        placeholderText: previewFps
                        validator: RegExpValidator {
                            regExp: /^[0-9]{1,2}$/
                        }
                        horizontalAlignment: TextInput.AlignHCenter
                        selectByMouse: true
                        text: previewFps
                        onTextChanged: {
                            if (acceptableInput) {
                                previewFps = text;
                            }
                        }
                    }

                    Label {
                        text: qsTr("Full-screen display")
                    }

                    TextField {
                        placeholderText: displayFps
                        validator: RegExpValidator {
                            regExp: /^[0-9]{1,2}$/
                        }
                        horizontalAlignment: TextInput.AlignHCenter
                        selectByMouse: true
                        text: displayFps
                        onTextChanged: {
                            if (acceptableInput) {
                                displayFps = text;
                            }
                        }
                    }
                }
            }

            Label {
                text: qsTr("Monitor selection")
            }

            GridView {
                Layout.fillWidth: true
                cellWidth: width / columns
                cellHeight: 40
                height: cellHeight * count
                model: columns * rows
                delegate: ItemDelegate {
                    id: item
                    width: GridView.view.cellWidth
                    height: GridView.view.cellHeight
                    onClicked: monitorSelect.open()

                    readonly property int index: model.index

                    Rectangle {
                        anchors {
                            fill: parent
                            margins: 1
                        }
                        border {
                            width: 1
                            color: Material.primaryColor
                        }
                        color: "transparent"

                        Label {
                            id: itemDisplay
                            anchors.centerIn: parent
                            text: monitors[index] || qsTr("Not selected")
                        }
                    }

                    Menu {
                        id: monitorSelect

                        MenuItem {
                            text: qsTr("-- Empty --");
                            onTriggered: {
                                monitors[item.index] = undefined;
                                itemDisplay.text = qsTr("Not selected");
                            }
                        }

                        Repeater {
                            model: MonitorListModel {}

                            MenuItem {
                                text: name
                                onTriggered: {
                                    monitors[item.index] = id;
                                    itemDisplay.text = id;
                                }
                            }
                        }
                    }
                }
            }

        }
    }

}
