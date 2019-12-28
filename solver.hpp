#pragma once

#include <minisat/core/Solver.h>

#include <vector>

#include "sudokusettings.hpp"

using Board = std::vector<std::vector<int>>;

class Solver {
private:
    const bool m_write_dimacs = false;
    Minisat::Solver solver;

public:
    Solver(Settings::Sudoku::BoardSettings board_settings, bool write_dimacs = false);
    // Returns true if applying the board does not lead to UNSAT result
    bool apply_board(Board const&);
    // Returns true if the sudoku has a solution
    bool solve();
    Board get_solution() const;

private:
    Minisat::Var toVar(int row, int column, int value) const;
    bool is_valid(Board const& b) const;

    void one_square_one_value();
    void non_duplicated_values();
    void exactly_one_true(Minisat::vec<Minisat::Lit> const& literals);
    void init_variables();

    Settings::Sudoku::BoardSettings m_board_settings;
};
