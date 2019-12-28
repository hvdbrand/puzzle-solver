#ifndef SUDOKUSETTINGS_HPP
#define SUDOKUSETTINGS_HPP

namespace Settings {
namespace Sudoku {

enum class Size
{
  SIZE_9X9,
  SIZE_16X16
};

struct BoardSettings
{
    int rows = 9;
    int columns = 9;
    int values = 9;
    int min_value = 0;
    int max_value = 9;
    int minibox_size = 3;
};

BoardSettings get_board_settings(Size size);

}

}

#endif // SUDOKUSETTINGS_HPP
