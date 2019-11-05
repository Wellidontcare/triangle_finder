cmake_minimum_required(VERSION 3.5)

project(triangle_finder LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(triangle_finder
    main.cpp
    main_window/mainwindow.h
    main_window/mainwindow.cpp
    custom_widget/sidebysideviewer.h
    custom_widget/sidebysideviewer.cpp
    custom_widget/stepwindow.h
    custom_widget/stepwindow.cpp
    custom_widget/dropenabledgraphicsview.h
    custom_widget/dropenabledgraphicsview.cpp
    triangle_finder_model/trianglefindermodel.h
    triangle_finder_model/trianglefindermodel.cpp
    triangle_finder_model/triangleinfocontainer.h
    triangle_finder_adapter/trianglefinderadapter.h
    triangle_finder_adapter/trianglefinderadapter.cpp
    main_window/mainwindow.ui
    resources.qrc
    )
add_definitions(${QT_DEFINITIONS})
target_link_libraries(triangle_finder
    ${OpenCV_LIBS}
    ${QT_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Core_LIBRARIES}
    ${Qt5Gui_LIBRARIES})
