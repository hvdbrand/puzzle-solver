#pragma once

#include "sudokusolver.hpp"

#include <QFile>

namespace PuzzleSaver {
    void save_to_file(const Settings::Sudoku::BoardSettings& board_settings, const SudokuBoard& example_board, QFile &file);

}
