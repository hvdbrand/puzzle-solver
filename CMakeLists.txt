cmake_minimum_required (VERSION 3.5)
project (puzzle-solver VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

find_package(MiniSat 2.2 REQUIRED)
find_package(Qt5 COMPONENTS Widgets Gui Core Xml REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(puzzle-solver
    sudokupuzzle.cpp
    sudokupuzzle.hpp
    puzzle_loader.cpp
    puzzle_loader.hpp
    puzzle_saver.cpp
    puzzle_saver.hpp
    puzzle_types.hpp
    sudokusolver.cpp
    sudokusolver.hpp
)

target_link_libraries(puzzle-solver
    MiniSat::libminisat
    Qt5::Core
    Qt5::Xml
)

add_executable(sudoku-cli
    main-cli.cpp
)

target_link_libraries(sudoku-cli
    puzzle-solver
)

qt5_wrap_ui(ui_wrap mainwindow.ui)
qt5_wrap_cpp(moc_sources mainwindow.h)

add_executable(puzzle-solver-gui
    main.cpp
    mainwindow.cpp
    mainwindow.h
    ${ui_wrap}
    ${moc_sources}
)

target_link_libraries(puzzle-solver-gui
    puzzle-solver
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets)
