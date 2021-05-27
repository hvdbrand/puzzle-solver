#pragma once

#include "sudokupuzzle.hpp"

#include <QFile>

namespace PuzzleLoader {
    std::unique_ptr<SudokuPuzzle> load_from_file(QFile& file);

}
