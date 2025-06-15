cmake_minimum_required(VERSION 3.5)

project(AquaSense VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Qt6::Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

# Find Python3 package (this is necessary to integrate Python Intepreter
find_package(Python3 COMPONENTS Interpreter Development)

# Set path to PySide6 include directory
set(PYTHON_VERSION "3.11")
set(PYTHON_INCLUDE_DIR "C:/Program Files/Python311/include")
set(PYTHON_SITE_PACKAGES_DIR "C:/Users/amank/AppData/Roaming/Python/Python${PYTHON_VERSION}/site-packages")

# Include PySide6 headers
include_directories(${PYSIDE6_INCLUDE_DIR})

# Include Python311 headers
include_directories(${PYTHON_INCLUDE_DIR})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AquaSense
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        assets.qrc
        assets/icon-1024x1024.png assets/icon-128x128.png assets/icon-432x432.png
        assets/icon-200x200.png
        scripts/greet.py
        data/data.csv
        scripts/visualize.py
        requirements.txt
        scripts/load_model.py
        scripts/random_forest_model.pkl
        scripts/scaler.pkl
        scripts/__init__.py
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET AquaSense APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(AquaSense SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(AquaSense
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(AquaSense PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Python3::Python Qt${Qt6_VERSION_MAcki}::Network PRIVATE Qt6::Core)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AquaSense)
endif()
set_target_properties(AquaSense PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AquaSense
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AquaSense)
endif()
