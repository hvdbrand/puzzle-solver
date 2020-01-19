#pragma once

#include <minisat/core/Solver.h>
#include <utility>
#include <vector>

using SudokuBoard = std::vector<std::vector<int>>;

namespace Settings {
namespace Sudoku {

enum class Type
{
    S9X9,
    S16X16,
    T9x15
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

BoardSettings get_board_settings(Type type);

bool get_sudoku_example(Type sudoku_type, SudokuBoard& example_board);

}
}

class SudokuSolver {
public:
    SudokuSolver(Settings::Sudoku::BoardSettings board_settings, bool write_dimacs = false);
    // Returns true if the provided board satisfies the constraints
    bool apply_board(SudokuBoard const&);
    // Returns true if the sudoku has a solution
    bool solve();
    SudokuBoard get_solution() const;

private:
    Minisat::Var toVar(int row, int column, int value) const;
    bool is_valid(SudokuBoard const& b) const;

    void one_square_one_value();
    void non_duplicated_values();
    void exactly_one_true(Minisat::vec<Minisat::Lit> const& literals);
    void init_variables();

    const bool m_write_dimacs = false;
    Minisat::Solver solver;
    Settings::Sudoku::BoardSettings m_board_settings;
};
