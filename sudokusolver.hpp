#pragma once

#include "puzzle_types.hpp"
#include <minisat/core/Solver.h>
#include <utility>
#include <vector>

namespace Settings {
namespace Sudoku {

enum class Type
{
    S9X9,
    S16X16,
    T9X15,
    M9X9_TWICE_COUPLED,
};

struct BoardSettings
{
    // TODO: Remove type when all examples are in board settings
    Type type = Type::S9X9;
    int rows;
    int columns;
    int values;
    int min_value;
    int max_value;
    RegionSet regions;
    PointSet empty_points;
    bool has_example;
    ExampleSet example;
};

BoardSettings get_board_settings(Type type);

bool get_sudoku_example(Type sudoku_type, Board& example_board);

}
}

class SudokuSolver {
public:
    SudokuSolver(Settings::Sudoku::BoardSettings board_settings, bool write_dimacs = false);
    // Returns true if the provided board satisfies the constraints
    bool apply_board(Board const&);
    // Returns true if the sudoku has a solution
    bool solve();
    Board get_solution() const;

private:
    Minisat::Var toVar(int row, int column, int value) const;
    bool is_valid(Board const& b) const;

    void one_square_one_value();
    void non_duplicated_values();
    void empty_points();
    void exactly_one_true(Minisat::vec<Minisat::Lit> const& literals);
    void init_variables();

    const bool m_write_dimacs = false;
    Minisat::Solver solver;
    Settings::Sudoku::BoardSettings m_board_settings;
};