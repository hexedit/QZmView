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

ItemDelegate {
    id: monitor

    property int monitorId: 0
    property string name: ""
    property bool single: false
    property int maxfps: 0

    Rectangle {
        id: back
        anchors {
            fill: parent
            margins: 1
        }

        border {
            color: Material.primaryColor
            width: 1
        }
        color: "black"

        Monitor {
            anchors {
                fill: parent
                margins: back.border.width
                topMargin: back.border.width + (name.implicitHeight * name.visible)
            }
            single: monitor.single
            maxfps: monitor.maxfps
            monitorId: monitor.monitorId
        }

        Label {
            id: name
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: back.border.width
            }
            background: Rectangle {
                color: "black";
            }
            color: "white"
            text: monitor.name
            visible: monitor.name.length > 0
        }
    }
}
