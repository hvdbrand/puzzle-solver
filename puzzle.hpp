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

    int get_values() const;
    int get_rows() const;
    int get_columns() const;
    RegionVector get_regions() const;

    bool replace_region(int i, Region& region);
    bool remove_region(int i);
    bool add_region(Region& region);

    // Returns true if the provided board satisfies the puzzle constraints
    bool apply_board(const Board& board);
    // Returns true if the puzzle has a solution
    bool solve();
    Board get_solution() const;

    bool has_example() const;
    Board get_example() const;

    Settings::Sudoku::BoardSettings get_board_settings();

private:
    Settings::Sudoku::BoardSettings m_board_settings;

    SudokuSolver* m_solver;
};
