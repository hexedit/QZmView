# QZmView

A Qt-based view-only ZoneMinder client

## What IS implemented

* Logging in using ZoneMinder authentication
* ZoneMinder token refresh (not fully tested)
* Monitor list with ability of live streaming
* Grids with configurable columns/rows and FPS

## What IS NOT implemented

* Log in using HTTP Basic authentication
* API without authentication
* List and view events for monitors
* Unit and integrations tests

## What HAS NOT TO BE implemented

Any write operations, such as modify/delete monitors, modify/delete events, system settings, etc.

## Requirements

* Qt version 5.12 and later
* Qt QuickControls 2

## Build/install

Build using CMake system

```c++
git clone https://github.com/hexedit/QZmView.git
cd QZmView
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/any/path/you/want
make
[sudo] make install
```

Using `sudo` for install is optional if you install to user-local folder

## Comments

This program is currently developed to resolve my local requirements. Inspired by [zmNinja](https://pliablepixels.github.io/) and [iSpy](https://www.ispyconnect.com/).
