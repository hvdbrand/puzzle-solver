#include "sudokusettings.hpp"

namespace Settings {
namespace Sudoku {

BoardSettings get_board_settings(Size size)
{
    BoardSettings board_settings;
    if (size == Size::SIZE_9X9)
    {
        board_settings.rows = 9;
        board_settings.columns = 9;
        board_settings.values = 9;
        board_settings.min_value = 0;
        board_settings.max_value = 9;
        board_settings.minibox_size = 3;
    }
    else if (size == Size::SIZE_16X16)
    {
        board_settings.rows = 16;
        board_settings.columns = 16;
        board_settings.values = 16;
        board_settings.min_value = 0;
        board_settings.max_value = 16;
        board_settings.minibox_size = 4;
    }
    return board_settings;
}

}
}
