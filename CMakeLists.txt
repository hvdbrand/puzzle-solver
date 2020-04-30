cmake_minimum_required (VERSION 3.5)
project (sudoku-SAT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

find_package(MiniSat 2.2 REQUIRED)
find_package(Qt5 COMPONENTS Widgets Gui Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(puzzle-solver
    puzzle.cpp
    puzzle.hpp
    puzzle_types.hpp
    sudokusolver.cpp
    sudokusolver.hpp
)

target_link_libraries(puzzle-solver
    MiniSat::libminisat
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
