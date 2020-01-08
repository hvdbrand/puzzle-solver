#include "sudokusettings.hpp"

namespace Settings {
namespace Sudoku {

BoardSettings get_board_settings(PuzzleType size)
{
    BoardSettings board_settings;
    if (size == PuzzleType::SUDOKU_9X9)
    {
        board_settings.rows = 9;
        board_settings.columns = 9;
        board_settings.values = 9;
        board_settings.min_value = 0;
        board_settings.max_value = 9;
        board_settings.minibox_size = 3;
        board_settings.horizontal_regions = { {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0}};
    }
    else if (size == PuzzleType::SUDOKU_16X16)
    {
        board_settings.rows = 16;
        board_settings.columns = 16;
        board_settings.values = 16;
        board_settings.min_value = 0;
        board_settings.max_value = 16;
        board_settings.minibox_size = 4;
        board_settings.horizontal_regions = { {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0}, {9,0}, {10,0}, {11,0}, {12,0}, {13,0}, {14,0}, {15,0}};
    }
    else if (size == PuzzleType::TWINSUDOKU_9X15)
    {
        board_settings.rows = 9;
        board_settings.columns = 15;
        board_settings.values = 9;
        board_settings.min_value = 0;
        board_settings.max_value = 9;
        board_settings.minibox_size = 3;
        board_settings.horizontal_regions = { {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0},
                                              {0,6}, {1,6}, {2,6}, {3,6}, {4,6}, {5,6}, {6,6}, {7,6}, {8,6}};
    }
    return board_settings;
}

}
}
