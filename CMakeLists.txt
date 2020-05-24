cmake_minimum_required(VERSION 3.5)

project(hex_utils LANGUAGES CXX)

find_package(Qt5 5.7 COMPONENTS Core Gui Widgets REQUIRED)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
  main.cpp
  main_window.cpp
  main_window.h
  main_window.ui
)
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  AUTOMOC ON
  AUTOUIC ON
  AUTORCC ON
  )

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  )

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  )
