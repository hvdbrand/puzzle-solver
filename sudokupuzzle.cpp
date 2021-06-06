#include "sudokupuzzle.hpp"
#include <map>

namespace {
const std::map<Settings::PuzzleType, Settings::Sudoku::Type> SUDOKU_TYPES {
    {Settings::PuzzleType::SUDOKU_9X9, Settings::Sudoku::Type::S9X9},
    {Settings::PuzzleType::SUDOKU_16X16, Settings::Sudoku::Type::S16X16},
    {Settings::PuzzleType::TWINSUDOKU_9X15, Settings::Sudoku::Type::T9X15},
    {Settings::PuzzleType::SUDOKU_MIX_9X9_TWICE, Settings::Sudoku::Type::M9X9_TWICE_COUPLED},
    };
}

SudokuPuzzle::SudokuPuzzle(Settings::PuzzleType predefined_puzzle_type)
    : m_board_settings(Settings::Sudoku::get_board_settings(SUDOKU_TYPES.at(predefined_puzzle_type)))
{
}

SudokuPuzzle::SudokuPuzzle(const Settings::Sudoku::BoardSettings& board_settings)
    : m_board_settings(board_settings)
{
}

SudokuPuzzle::~SudokuPuzzle()
{
}

int SudokuPuzzle::get_values() const
{
    return m_board_settings.values;
}

int SudokuPuzzle::get_rows() const
{
    return m_board_settings.rows;
}

int SudokuPuzzle::get_columns() const
{
    return m_board_settings.columns;
}

RegionVector SudokuPuzzle::get_regions() const
{
    return m_board_settings.regions;
}

void SudokuPuzzle::replace_region(int i, Region& region)
{
    assert((i >= 0) && (i < m_board_settings.regions.size()));
    m_board_settings.regions[i] = region;
}

void SudokuPuzzle::remove_region(int i)
{
    assert((i >= 0) && (i < m_board_settings.regions.size()));
    m_board_settings.regions.erase(m_board_settings.regions.begin() +  i);
}

void SudokuPuzzle::add_region(Region& region)
{
    m_board_settings.regions.push_back(region);
}

bool SudokuPuzzle::apply_board(const SudokuBoard& board)
{
    m_solver = std::make_unique<SudokuSolver>(m_board_settings);
    return m_solver->apply_board(board);
}

bool SudokuPuzzle::solve()
{
    if (!m_solver)
        return false;
    return m_solver->solve();
}

SudokuBoard SudokuPuzzle::get_solution() const
{
    if (!m_solver)
        return SudokuBoard(m_board_settings.rows, std::vector<Value>(m_board_settings.columns));
    return m_solver->get_solution();
}

bool SudokuPuzzle::has_example() const
{
    return m_board_settings.has_example;
}

SudokuBoard SudokuPuzzle::get_example() const
{
    SudokuBoard board(m_board_settings.rows, std::vector<Value>(m_board_settings.columns));
    for (auto examplePoint : m_board_settings.example)
    {
        board[examplePoint.first.first][examplePoint.first.second] = examplePoint.second;
    }
    return board;
}

Settings::Sudoku::BoardSettings SudokuPuzzle::get_board_settings()
{
    return m_board_settings;
}
