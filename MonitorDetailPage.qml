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

    property int monitorId: 0
    property string monitorName: ""

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
            text: monitorName
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }

    Monitor {
        anchors.fill: parent
        monitorId: root.monitorId

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: root.StackView.view.pop()
        }
    }
}
