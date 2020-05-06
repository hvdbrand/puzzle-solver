#pragma once

#include "puzzle.hpp"

#include <QFile>

namespace PuzzleLoader {
    std::unique_ptr<Puzzle> load_from_file(QFile& file);

}
