#pragma once

#include "sudokusolver.hpp"
#include <utility>
#include <vector>

namespace Settings {

enum class PuzzleType
{
  SUDOKU_9X9,
  SUDOKU_16X16,
  TWINSUDOKU_9X15
};

}

using Board = std::vector<std::vector<int>>;

class Puzzle
{
public:
    Puzzle(Settings::PuzzleType puzzle_type);
    virtual ~Puzzle();

    int getRows() const;
    int getColumns() const;

    // The methods below are still Sudoku specific
    bool isTwinSudoku() const;
    int getMiniboxSize() const;

    // Returns true if the provided board satisfies the puzzle constraints
    bool apply_board(const Board& board);
    // Returns true if the puzzle has a solution
    bool solve();
    Board get_solution() const;
    // Returns a true if an example is set in the board variable
    bool get_example(Board& board) const;
private:
    Settings::PuzzleType m_puzzle_type;
    Settings::Sudoku::BoardSettings m_board_settings;

    SudokuSolver* m_solver;
};
