function(windeployqt TARGET)
    find_program(CMAKE_WINDEPLOYQT windeployqt PATHS "$ENV{QTDIR}/bin")
    if(NOT CMAKE_WINDEPLOYQT)
        message(FATAL_ERROR "windeployqt not found")
    endif()

    cmake_parse_arguments(WINDEPLOYQT
        ""
        "DESTDIR;QMLDIR"
        ""
        ${ARGN})

    if(Qt5Core_VERSION VERSION_LESS 5.14.0)
        if($<LOWER_CASE:${CMAKE_BUILD_TYPE}> STREQUAL "debug")
            list(APPEND CMAKE_WINDEPLOYQT_FLAGS "--debug")
        else()
            list(APPEND CMAKE_WINDEPLOYQT_FLAGS "--release")
        endif()
    endif()

    if(WINDEPLOYQT_DESTDIR)
        list(APPEND CMAKE_WINDEPLOYQT_FLAGS "--dir")
        list(APPEND CMAKE_WINDEPLOYQT_FLAGS "${WINDEPLOYQT_DESTDIR}")
    endif()

    if(WINDEPLOYQT_QMLDIR)
        list(APPEND CMAKE_WINDEPLOYQT_FLAGS "--qmldir")
        list(APPEND CMAKE_WINDEPLOYQT_FLAGS "${WINDEPLOYQT_QMLDIR}")
    endif()

    add_custom_command(
        TARGET ${TARGET}
        POST_BUILD COMMAND ${CMAKE_WINDEPLOYQT}
        ${CMAKE_WINDEPLOYQT_FLAGS}
        $<TARGET_FILE:${TARGET}>)
endfunction()
