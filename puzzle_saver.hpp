#pragma once

#include "sudokusolver.hpp"

namespace PuzzleSaver {
    bool save_to_file(const Settings::Sudoku::BoardSettings& board_settings, const Board& example_board);

}
