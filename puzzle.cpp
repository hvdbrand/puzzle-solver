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

Puzzle::Puzzle(Settings::PuzzleType predefined_puzzle_type)
    : m_board_settings(Settings::Sudoku::get_board_settings(SUDOKU_TYPES.at(predefined_puzzle_type)))
    , m_solver(NULL)
{
}

Puzzle::Puzzle(const Settings::Sudoku::BoardSettings& board_settings)
    : m_board_settings(board_settings)
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

int Puzzle::get_values() const
{
    return m_board_settings.values;
}

int Puzzle::get_rows() const
{
    return m_board_settings.rows;
}

int Puzzle::get_columns() const
{
    return m_board_settings.columns;
}

RegionVector Puzzle::get_regions() const
{
    return m_board_settings.regions;
}

bool Puzzle::replace_region(int i, Region& region)
{
    m_board_settings.regions[i] = region;
}

bool Puzzle::remove_region(int i)
{
    m_board_settings.regions.erase(m_board_settings.regions.begin() +  i);
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
        return Board(m_board_settings.rows, std::vector<Value>(m_board_settings.columns));
    return m_solver->get_solution();
}

bool Puzzle::has_example() const
{
    return m_board_settings.has_example;
}

Board Puzzle::get_example() const
{
    Board board(m_board_settings.rows, std::vector<Value>(m_board_settings.columns));
    if (m_board_settings.example.size() > 0)
    {
        for (auto examplePoint : m_board_settings.example)
        {
            board[examplePoint.first.first][examplePoint.first.second] = examplePoint.second;
        }
        return board;
    }
    // TODO: Move separate examples from get_sudoku_example to board_settings
    Settings::Sudoku::get_sudoku_example(m_board_settings.type, board);
    return board;
}

Settings::Sudoku::BoardSettings Puzzle::get_board_settings()
{
    return m_board_settings;
}
