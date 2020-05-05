#pragma once

#include "puzzle_types.hpp"
#include "sudokusolver.hpp"
#include <memory>

namespace Settings {

enum class PuzzleType
{
  SUDOKU_9X9,
  SUDOKU_16X16,
  TWINSUDOKU_9X15,
  SUDOKU_MIX_9X9_TWICE,
};

}

class Puzzle
{
public:
    Puzzle(Settings::PuzzleType predefined_puzzle_type);
    Puzzle(const Settings::Sudoku::BoardSettings& board_settings);
    virtual ~Puzzle();

    int getRows() const;
    int getColumns() const;
    RegionSet getRegions() const;

    // Returns true if the provided board satisfies the puzzle constraints
    bool apply_board(const Board& board);
    // Returns true if the puzzle has a solution
    bool solve();
    Board get_solution() const;

    bool has_example() const;
    Board get_example() const;

private:
    Settings::Sudoku::BoardSettings m_board_settings;

    SudokuSolver* m_solver;
};
