#pragma once

#include "puzzle_types.hpp"
#include <memory>

class GcSolver;

class GcPuzzle
{
public:
    static const int GC_HORIZONTAL_CELL_COUNT = 10;
    static const int GC_VERTICAL_CELL_COUNT = 11;
    static const int GC_HORIZONTAL_LINE_COUNT = GC_VERTICAL_CELL_COUNT + 1;
    static const int GC_VERTICAL_LINE_COUNT = GC_HORIZONTAL_CELL_COUNT + 1;

    GcPuzzle();
    virtual ~GcPuzzle();
    bool apply_board(const GcBoard& board);
    // Returns true if the puzzle has a solution
    bool solve();
    GcBoard get_solution() const;

private:
    std::unique_ptr<GcSolver> m_solver;
};
