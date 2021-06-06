#pragma once

#include "puzzle_types.hpp"
#include "sudokusolver.hpp"
#include <memory>

class SudokuPuzzle
{
public:
    SudokuPuzzle(Settings::PuzzleType predefined_puzzle_type);
    SudokuPuzzle(const Settings::Sudoku::BoardSettings& board_settings);
    virtual ~SudokuPuzzle();

    int get_values() const;
    int get_rows() const;
    int get_columns() const;
    RegionVector get_regions() const;

    void replace_region(int i, Region& region);
    void remove_region(int i);
    void add_region(Region& region);

    // Returns true if the provided board satisfies the puzzle constraints
    bool apply_board(const SudokuBoard& board);
    // Returns true if the puzzle has a solution
    bool solve();
    SudokuBoard get_solution() const;

    bool has_example() const;
    SudokuBoard get_example() const;

    Settings::Sudoku::BoardSettings get_board_settings();

private:
    Settings::Sudoku::BoardSettings m_board_settings;

    std::unique_ptr<SudokuSolver> m_solver;
};
