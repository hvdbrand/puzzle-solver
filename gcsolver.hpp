#pragma once

#include "puzzle_types.hpp"
#include <minisat/core/Solver.h>
#include <utility>
#include <vector>

struct GcSolverSettings
{
    int horizontal_cell_count;
    int horizontal_line_count;
    int vertical_cell_count;
    int vertical_line_count;
};

class GcSolver {
public:
    GcSolver(GcSolverSettings solver_settings, bool write_dimacs = false);
    // Returns true if the provided board satisfies the constraints
    bool apply_board(const GcBoard& board);
    // Returns true if the gc puzzle has a solution
    bool solve();
    GcBoard get_solution() const;

private:
    Minisat::Var toVar(int line, int segment, bool horizontal) const;
    bool is_valid(const GcBoard& board) const;

    void init_variables();
    void add_clauses_for_segments_surrounding_cells();
    void add_clauses_for_corners();
    void none_true(Minisat::vec<Minisat::Lit> const& literals);
    void exactly_one_true(Minisat::vec<Minisat::Lit> const& literals);
    void exactly_two_true(Minisat::vec<Minisat::Lit> const& literals);
    void exactly_three_true(Minisat::vec<Minisat::Lit> const& literals);
    void none_or_both(Minisat::vec<Minisat::Lit> const& literals);

    const bool m_write_dimacs = false;
    Minisat::Solver m_solver;

    const GcSolverSettings m_solver_settings;
    std::vector<std::vector<int>> m_adjacents;
};
