#pragma once

#include "puzzle.hpp"

namespace PuzzleLoader {
    std::unique_ptr<Puzzle> load_from_file();

}
