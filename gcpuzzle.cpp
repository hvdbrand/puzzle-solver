#include "gcpuzzle.hpp"
#include "gcsolver.hpp"

GcPuzzle::GcPuzzle()
{
}

GcPuzzle::~GcPuzzle()
{

}

bool GcPuzzle::apply_board(const GcBoard& board)
{
    GcSolverSettings settings;
    settings.horizontal_line_count = GC_HORIZONTAL_LINE_COUNT;
    settings.horizontal_cell_count = GC_HORIZONTAL_CELL_COUNT;
    settings.vertical_line_count = GC_VERTICAL_LINE_COUNT;
    settings.vertical_cell_count = GC_VERTICAL_CELL_COUNT;
    m_solver = std::make_unique<GcSolver>(settings);
    return m_solver->apply_board(board);
}

bool GcPuzzle::solve()
{
    if (!m_solver)
        return false;
    return m_solver->solve();
}

GcBoard GcPuzzle::get_solution() const
{
    if (!m_solver)
        return GcBoard();
    return m_solver->get_solution();
}
