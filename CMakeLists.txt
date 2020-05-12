cmake_minimum_required(VERSION 3.5)

project(QZmView VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_LIBS
    Core
    QuickControls2
    Widgets
    Network
)

set(HEADERS
    qzmsettings.h
    zmtoken.h
    zmapi.h
    zmmonitor.h
    monitorlistmodel.h
    gridlistmodel.h
    qzmmonitor.h
)

set(SOURCES
    main.cpp
    qzmsettings.cpp
    zmtoken.cpp
    zmapi.cpp
    zmmonitor.cpp
    monitorlistmodel.cpp
    gridlistmodel.cpp
    qzmmonitor.cpp
)

set(QML_SOURCES
    qml.qrc
)

include(GNUInstallDirs)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    list(APPEND SOURCES win32res.rc)
endif()

find_package(Qt5 5.12 COMPONENTS ${QT_LIBS} REQUIRED)

if(ANDROID)
    add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES} ${QML_SOURCES})
else()
    add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${QML_SOURCES})
endif()

target_compile_definitions(${PROJECT_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
foreach(qtlib ${QT_LIBS})
    target_link_libraries(${PROJECT_NAME}
      PRIVATE Qt5::${qtlib})
endforeach()

install(TARGETS ${PROJECT_NAME}
    DESTINATION $<IF:$<BOOL:${WIN32}>,${CMAKE_INSTALL_PREFIX},${CMAKE_INSTALL_BINDIR}>)

if(WIN32)
    include(cmake/windeployqt.cmake)
    windeployqt(${PROJECT_NAME}
        DESTDIR "${PROJECT_BINARY_DIR}/deploy"
        QMLDIR "${CMAKE_CURRENT_SOURCE_DIR}")
    install(DIRECTORY "${PROJECT_BINARY_DIR}/deploy/"
        DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
