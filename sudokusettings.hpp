#ifndef SUDOKUSETTINGS_HPP
#define SUDOKUSETTINGS_HPP

#include <utility>
#include <vector>

namespace Settings {
namespace Sudoku {

enum class PuzzleType
{
  SUDOKU_9X9,
  SUDOKU_16X16,
  TWINSUDOKU_9X15
};

struct BoardSettings
{
    int rows = 9;
    int columns = 9;
    int values = 9;
    int min_value = 0;
    int max_value = 9;
    int minibox_size = 3;
    std::vector<std::pair<int, int>> horizontal_regions = { {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0} };
};

BoardSettings get_board_settings(PuzzleType size);

}

}

#endif // SUDOKUSETTINGS_HPP
