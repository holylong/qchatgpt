cmake_minimum_required(VERSION 3.5)

project(chatgpt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Network LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network LinguistTools REQUIRED)

set(TS_FILES chatgpt_zh_CN.ts)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qmarkdowntextedit)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qmarkdowntextedit)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h

#        markdownwidget.h markdownwidget.cpp
#        downloader.h downloader.cpp
        httprequest.h httprequest.cpp
        chatgptconfig.h chatgptconfig.cpp
        settingdialog.h settingdialog.cpp
        fileutil.h fileutil.cpp
        updatetextworker.h updatetextworker.cpp
        mainwindow.ui
        app.qrc
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(chatgpt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(chatgpt SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(chatgpt
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(chatgpt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(chatgpt PRIVATE qmarkdowntextedit)

set_target_properties(chatgpt PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(chatgpt)
endif()
