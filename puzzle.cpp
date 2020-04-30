#include "puzzle.hpp"
#include <map>

namespace {
const std::map<Settings::PuzzleType, Settings::Sudoku::Type> SUDOKU_TYPES {
    {Settings::PuzzleType::SUDOKU_9X9, Settings::Sudoku::Type::S9X9},
    {Settings::PuzzleType::SUDOKU_16X16, Settings::Sudoku::Type::S16X16},
    {Settings::PuzzleType::TWINSUDOKU_9X15, Settings::Sudoku::Type::T9X15},
    {Settings::PuzzleType::SUDOKU_MIX_9X9_TWICE, Settings::Sudoku::Type::M9X9_TWICE_COUPLED},
    };
}

Puzzle::Puzzle(Settings::PuzzleType puzzle_type)
    : m_puzzle_type(puzzle_type)
    , m_board_settings(Settings::Sudoku::get_board_settings(SUDOKU_TYPES.at(m_puzzle_type)))
    , m_solver(NULL)
{
}

Puzzle::~Puzzle()
{
    if (m_solver != NULL)
    {
        delete m_solver;
    }
}

int Puzzle::getRows() const
{
    return m_board_settings.rows;
}

int Puzzle::getColumns() const
{
    return m_board_settings.columns;
}

RegionSet Puzzle::getRegions() const
{
    return m_board_settings.adjacent_regions;
}

bool Puzzle::apply_board(const Board& board)
{
    if (m_solver != NULL)
    {
        delete m_solver;
    }
    m_solver = new SudokuSolver(m_board_settings);
    return m_solver->apply_board(board);
}

bool Puzzle::solve()
{
    if (m_solver == NULL)
        return false;
    return m_solver->solve();
}

Board Puzzle::get_solution() const
{
    if (m_solver == NULL)
        return Board(m_board_settings.rows, std::vector<int>(m_board_settings.columns));
    return m_solver->get_solution();
}

bool Puzzle::get_example(Board& board) const
{
    bool found_example = false;
    auto it_sudoku_type = SUDOKU_TYPES.find(m_puzzle_type);
    if (it_sudoku_type != SUDOKU_TYPES.end())
    {
        found_example = Settings::Sudoku::get_sudoku_example(it_sudoku_type->second, board);
    }
    return found_example;
}
