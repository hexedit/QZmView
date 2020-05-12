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
import QtQuick.Layouts 1.12
import QZmView 1.0
import "IconFont.js" as IconFont

Page {
    id: root

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
            text: qsTr("Settings")
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
                enabled = false;

                // ZM settings
                Settings.zmBaseUrl = zmBaseUrl.text;
                Settings.zmApiUrl = zmApiUrl.text;
                Settings.zmCgiUrl = zmCgiUrl.text;
                Settings.zmEnableAuth = zmAuthEnable.checked
                Settings.isZmAuth = zmAuthZm.checked && !zmAuthHttp.checked;
                Settings.authUsername = zmUsername.text;
                Settings.authPassword = zmPassword.text;

                // UI settings
                Settings.darkTheme = uiDarkTheme.checked;
                if (uiAutoStart.enabled) {
                    Settings.autoStart = uiAutoStart.checked;
                }
                Settings.runToTray = uiRunToTray.checked;
                Settings.closeToTray = uiCloseToTray.checked;
                Settings.listMotion = uiListMotion.checked;
                if (uiListMaxFps.acceptableInput) {
                    Settings.listMaxFps = uiListMaxFps.text;
                }

                enabled = true;
            }
        }
    }

    TabBar {
        id: tabs
        width: parent.width

        TabButton {
            text: "ZoneMinder"
        }

        TabButton {
            text: qsTr("User Interface")
        }
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        anchors.margins: 10
        anchors.topMargin: tabs.implicitHeight + 10;
        clip: true

        StackLayout {
            width: Math.max(implicitWidth, scrollView.availableWidth)
            currentIndex: tabs.currentIndex

            ColumnLayout {
                width: parent.width

                GroupBox {
                    Layout.fillWidth: true
                    title: qsTr("ZoneMinder address")

                    ColumnLayout {
                        anchors.fill: parent;

                        Label {
                            text: qsTr("ZoneMinder base URL")
                        }

                        TextField {
                            id: zmBaseUrl
                            Layout.fillWidth: true
                            selectByMouse: true
                            text: Settings.zmBaseUrl
                            placeholderText: qsTr("http://zm.example.com")
                            onTextChanged: {
                                zmApiUrl.text = text + "/api"
                                zmCgiUrl.text = text + "/cgi-bin"
                            }
                        }

                        Label {
                            text: qsTr("ZoneMinder API URL")
                        }

                        TextField {
                            id: zmApiUrl
                            Layout.fillWidth: true
                            selectByMouse: true
                            text: Settings.zmApiUrl
                            placeholderText: qsTr("http://zm.example.com/api/")
                        }

                        Label {
                            text: qsTr("ZoneMinder CGI URL")
                        }

                        TextField {
                            id: zmCgiUrl
                            Layout.fillWidth: true
                            selectByMouse: true
                            text: Settings.zmCgiUrl
                            placeholderText: qsTr("http://zm.example.com/cgi-bin/")
                        }
                    }
                }

                GroupBox {
                    Layout.fillWidth: true
                    title: qsTr("Authentication")

                    ColumnLayout {
                        anchors.fill: parent

                        CheckBox {
                            id: zmAuthEnable
                            text: qsTr("Enable")
                            checked: Settings.zmEnableAuth
                        }

                        Label {
                            text: qsTr("Username")
                            enabled: zmAuthEnable.checked
                        }

                        TextField {
                            id: zmUsername
                            Layout.fillWidth: true
                            selectByMouse: true
                            text: Settings.authUsername
                            placeholderText: qsTr("Username")
                            enabled: zmAuthEnable.checked
                        }

                        Label {
                            text: qsTr("Password")
                            enabled: zmAuthEnable.checked
                        }

                        TextField {
                            id: zmPassword
                            Layout.fillWidth: true
                            selectByMouse: true
                            text: Settings.authPassword
                            placeholderText: qsTr("Password")
                            echoMode: TextField.Password
                            enabled: zmAuthEnable.checked
                        }

                        GroupBox {
                            title: qsTr("Mode")
                            enabled: zmAuthEnable.checked
                            Layout.fillWidth: true

                            ButtonGroup {
                                id: zmAuthMode
                            }

                            RowLayout {
                                anchors.fill: parent

                                RadioButton {
                                    id: zmAuthZm
                                    text: qsTr("ZoneMinder")
                                    checked: Settings.isZmAuth
                                    ButtonGroup.group: zmAuthMode
                                }

                                RadioButton {
                                    id: zmAuthHttp
                                    text: qsTr("HTTP Basic")
                                    checked: !Settings.isZmAuth
                                    ButtonGroup.group: zmAuthMode
                                }
                            }

                        }

                    }

                }

            }

            ColumnLayout {
                width: parent.width

                CheckBox {
                    id: uiDarkTheme
                    text: qsTr("Use dark theme")
                    checked: Settings.darkTheme
                }

                GroupBox {
                    title: qsTr("Behavior")
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent

                        CheckBox {
                            id: uiAutoStart
                            text: qsTr("Run program on system start")
                            checked: Settings.autoStart
                            enabled: Settings.autoStart !== undefined
                        }

                        CheckBox {
                            id: uiRunToTray
                            text: qsTr("Run minimized to system tray")
                            checked: Settings.runToTray
                        }

                        CheckBox {
                            id: uiCloseToTray
                            text: qsTr("Close minimizes to system tray")
                            checked: Settings.closeToTray
                        }
                    }
                }

                GroupBox {
                    title: qsTr("Monitor list")
                    Layout.fillWidth: true

                    RowLayout {
                        anchors.fill: parent
                        spacing: 10

                        CheckBox {
                            id: uiListMotion
                            text: qsTr("Enable live motion")
                            checked: Settings.listMotion
                        }

                        Label {
                            text: qsTr("Maximum FPS")
                            enabled: uiListMotion.checked
                        }

                        TextField {
                            id: uiListMaxFps
                            selectByMouse: true
                            placeholderText: Settings.listMaxFps
                            text: Settings.listMaxFps
                            validator: RegExpValidator {
                                regExp: /^[0-9]{1,2}$/;
                            }
                            enabled: uiListMotion.checked
                        }
                    }
                }

            }

        }

    }

}
