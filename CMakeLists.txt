cmake_minimum_required(VERSION 3.6)
project(schaken)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

#=====================================================
# TODO: Change directory QT for windows/mac/linux
# Uncomment correct directory, and comment other directories
#=====================================================

#Windows defaults:
#set(Qt5Core_DIR "C:\\Qt\\5.9.7\\mingw53_32\\lib\\cmake\\Qt5Core")
#set(Qt5Widgets_DIR "C:\\Qt\\5.9.7\\mingw53_32\\lib\\cmake\\Qt5Widgets")
#set(Qt5Gui_DIR "C:\\Qt\\5.9.7\\mingw53_32\\lib\\cmake\\Qt5Gui")
#set(QT_QMAKE_EXECUTABLE "C:\\Qt\\5.9.7\\mingw53_32\\bin\\qmake.exe")

#Mac defailts:
#set(Qt5Core_DIR "~/Qt/5.9.7/clang_64/lib/cmake/Qt5Core")
#set(Qt5Widgets_DIR "~/Qt/5.9.7/clang_64/lib/cmake/Qt5Widgets")
#set(Qt5Gui_DIR "~/Qt/5.9.7/clang_64/lib/cmake/Qt5Gui")

#Linux:
set(Qt5Core_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt5Core")
set(Qt5Widgets_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt5Widgets")
set(Qt5Gui_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt5Gui")


find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp mainwindow.cpp mainwindow.h chessboard.cpp chessboard.h game.cpp game.h SchaakStuk.cpp SchaakStuk.h Positie.cpp Positie.h resources.h)
add_executable(schaken ${SOURCE_FILES})

qt5_use_modules(schaken Core Widgets Gui)
