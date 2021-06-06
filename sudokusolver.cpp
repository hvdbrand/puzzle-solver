#include "sudokusolver.hpp"

#include <iostream>

namespace Settings {
namespace Sudoku {

namespace {
    PointVector getMinibox(int row, int column, int size)
    {
        PointVector points;
        for (int rr = 0; rr < size; ++rr) {
            for (int cc = 0; cc < size; ++cc) {
                points.emplace_back(row + rr, column + cc);
            }
        }
        return points;
    }

    PointVector getHorizontalRegion(int row, int column, int size)
    {
        PointVector points;
        for (int cc = 0; cc < size; ++cc) {
            points.emplace_back(row, column + cc);
        }
        return points;
    }

    PointVector getVerticalRegion(int row, int column, int size)
    {
        PointVector points;
        for (int rr = 0; rr < size; ++rr) {
            points.emplace_back(row + rr, column);
        }
        return points;
    }
}

BoardSettings get_board_settings(Type type)
{
    BoardSettings board_settings;
    // TODO: Remove if all examples are part of the board settings
    board_settings.type = type;
    if (type == Type::S9X9)
    {
        board_settings.rows = 9;
        board_settings.columns = 9;
        board_settings.values = 9;
        board_settings.min_value = 0;
        board_settings.max_value = 9;
        board_settings.regions = { {getHorizontalRegion(0,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(1,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(2,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(3,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(4,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(5,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(6,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(7,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(8,0,9), PuzzleColor::NONE},

                                   {getVerticalRegion(0,0,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,1,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,2,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,3,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,4,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,5,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,6,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,7,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,8,9), PuzzleColor::NONE},

                                   {getMinibox(0,0,3), PuzzleColor::A},
                                   {getMinibox(0,3,3), PuzzleColor::B},
                                   {getMinibox(0,6,3), PuzzleColor::A},

                                   {getMinibox(3,0,3), PuzzleColor::B},
                                   {getMinibox(3,3,3), PuzzleColor::A},
                                   {getMinibox(3,6,3), PuzzleColor::B},

                                   {getMinibox(6,0,3), PuzzleColor::A},
                                   {getMinibox(6,3,3), PuzzleColor::B},
                                   {getMinibox(6,6,3), PuzzleColor::A},
                                 };
        board_settings.has_example = true;
        board_settings.example = { {{0,1}, 2}, {{0,2}, 6}, {{0,6}, 8}, {{0,7}, 1},
                                   {{1,0}, 3}, {{1,3}, 7}, {{1,5}, 8}, {{1,8}, 6},
                                   {{2,0}, 4}, {{2,4}, 5}, {{2,8}, 7},
                                   {{3,1}, 5}, {{3,3}, 1}, {{3,5}, 7}, {{3,7}, 9},
                                   {{4,2}, 3}, {{4,3}, 9}, {{4,5}, 5}, {{4,6}, 1},
                                   {{5,1}, 4}, {{5,3}, 3}, {{5,5}, 2}, {{5,7}, 5},
                                   {{6,0}, 1}, {{6,4}, 3}, {{6,8}, 2},
                                   {{7,0}, 5}, {{7,3}, 2}, {{7,5}, 4}, {{7,8}, 9},
                                   {{8,1}, 3}, {{8,2}, 8}, {{8,6}, 4}, {{8,7}, 6},
                                 };
    }
    else if (type == Type::S16X16)
    {
        board_settings.rows = 16;
        board_settings.columns = 16;
        board_settings.values = 16;
        board_settings.min_value = 0;
        board_settings.max_value = 16;
        board_settings.regions = { {getHorizontalRegion( 0,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion( 1,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion( 2,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion( 3,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion( 4,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion( 5,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion( 6,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion( 7,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion( 8,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion( 9,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion(10,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion(11,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion(12,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion(13,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion(14,0,16), PuzzleColor::NONE},
                                   {getHorizontalRegion(15,0,16), PuzzleColor::NONE},

                                   {getVerticalRegion(0, 0,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 1,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 2,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 3,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 4,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 5,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 6,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 7,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 8,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 9,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0,10,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0,11,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0,12,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0,13,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0,14,16), PuzzleColor::NONE},
                                   {getVerticalRegion(0,15,16), PuzzleColor::NONE},

                                   {getMinibox( 0, 0,4), PuzzleColor::A},
                                   {getMinibox( 0, 4,4), PuzzleColor::B},
                                   {getMinibox( 0, 8,4), PuzzleColor::A},
                                   {getMinibox( 0,12,4), PuzzleColor::B},

                                   {getMinibox( 4, 0,4), PuzzleColor::B},
                                   {getMinibox( 4, 4,4), PuzzleColor::A},
                                   {getMinibox( 4, 8,4), PuzzleColor::B},
                                   {getMinibox( 4,12,4), PuzzleColor::A},

                                   {getMinibox( 8, 0,4), PuzzleColor::A},
                                   {getMinibox( 8, 4,4), PuzzleColor::B},
                                   {getMinibox( 8, 8,4), PuzzleColor::A},
                                   {getMinibox( 8,12,4), PuzzleColor::B},

                                   {getMinibox(12, 0,4), PuzzleColor::B},
                                   {getMinibox(12, 4,4), PuzzleColor::A},
                                   {getMinibox(12, 8,4), PuzzleColor::B},
                                   {getMinibox(12,12,4), PuzzleColor::A},
                                 };
        board_settings.has_example = true;
        board_settings.example = {{{0,1}, 3}, {{0,2}, 11}, {{0,6}, 10}, {{0,7}, 16}, {{0,13}, 5}, {{0,14},6}, {{0,15}, 4},
                                  {{1,4}, 3}, {{1,9}, 11}, {{1,10}, 6}, {{1,12}, 14}, {{1,14}, 15}, {{1,15}, 1},
                                  {{2,2}, 1}, {{2,4}, 7}, {{2,5}, 15}, {{2,8}, 3}, {{2,9}, 14}, {{2,11}, 16}, {{2,12}, 12},
                                  {{3,1}, 14}, {{3,4}, 8}, {{3,5}, 12}, {{3,6}, 6}, {{3,10}, 13}, {{3,12}, 16}, {{3,13}, 9},
                                  {{4,1}, 4}, {{4,6}, 8}, {{4,7}, 12}, {{4,11}, 7}, {{4,13}, 3}, {{4,14}, 14}, {{4,15}, 15},
                                  {{5,0}, 10}, {{5,3}, 14}, {{5,4}, 11}, {{5,5}, 3}, {{5,8}, 9}, {{5,9},8}, {{5,14}, 7},
                                  {{6,0}, 3}, {{6,1}, 2}, {{6,2}, 12}, {{6,3}, 7}, {{6,4}, 16}, {{6,5}, 9}, {{6,9}, 6},
                                  {{7,0}, 13}, {{7,4}, 14}, {{7,6}, 7}, {{7,9}, 2}, {{7,13}, 6},
                                  {{8,2}, 8}, {{8,6}, 14}, {{8,9}, 7}, {{8,11}, 4}, {{8,15}, 13},
                                  {{9,6}, 16}, {{9,10}, 15}, {{9,11}, 8}, {{9,12}, 4}, {{9,13}, 11}, {{9,14}, 3}, {{9,15}, 2},
                                  {{10,1}, 16}, {{10,6}, 13}, {{10,7}, 8}, {{10,10}, 5}, {{10,11}, 14}, {{10,12}, 15}, {{10,15}, 7},
                                  {{11,0}, 12}, {{11,1}, 9}, {{11,2}, 3}, {{11,4}, 4}, {{11,8}, 2}, {{11,9}, 13}, {{11,14}, 5},
                                  {{12,2}, 13}, {{12,3}, 10}, {{12,5}, 8}, {{12,9}, 12}, {{12,10}, 7}, {{12,11}, 11}, {{12,14}, 16},
                                  {{13,3},1}, {{13,4},9}, {{13,6}, 5}, {{13,7}, 11}, {{13,10}, 14}, {{13,11}, 6}, {{13,13}, 7},
                                  {{14,0}, 5}, {{14,1}, 7}, {{14,3}, 8}, {{14,5}, 4}, {{14,6}, 12}, {{14,11}, 2},
                                  {{15,0}, 4}, {{15,1}, 11}, {{15,2}, 14}, {{15,8}, 8}, {{15,9}, 15}, {{15,13}, 13}, {{15,14}, 10},
                                 };
    }
    else if (type == Type::T9X15)
    {
        board_settings.rows = 9;
        board_settings.columns = 15;
        board_settings.values = 9;
        board_settings.min_value = 0;
        board_settings.max_value = 9;
        board_settings.regions = { {getHorizontalRegion(0,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(1,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(2,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(3,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(4,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(5,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(6,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(7,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(8,0,9), PuzzleColor::NONE},

                                   {getHorizontalRegion(0,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(1,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(2,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(3,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(4,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(5,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(6,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(7,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(8,6,9), PuzzleColor::NONE},

                                   {getVerticalRegion(0, 0,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 1,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 2,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 3,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 4,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 5,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 6,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 7,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 8,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0, 9,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,10,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,11,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,12,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,13,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,14,9), PuzzleColor::NONE},

                                   {getMinibox(0, 0,3), PuzzleColor::A},
                                   {getMinibox(0, 3,3), PuzzleColor::B},
                                   {getMinibox(0, 6,3), PuzzleColor::A_DARK},
                                   {getMinibox(0, 9,3), PuzzleColor::B},
                                   {getMinibox(0,12,3), PuzzleColor::A},

                                   {getMinibox(3, 0,3), PuzzleColor::B},
                                   {getMinibox(3, 3,3), PuzzleColor::A},
                                   {getMinibox(3, 6,3), PuzzleColor::B_DARK},
                                   {getMinibox(3, 9,3), PuzzleColor::A},
                                   {getMinibox(3,12,3), PuzzleColor::B},

                                   {getMinibox(6, 0,3), PuzzleColor::A},
                                   {getMinibox(6, 3,3), PuzzleColor::B},
                                   {getMinibox(6, 6,3), PuzzleColor::A_DARK},
                                   {getMinibox(6, 9,3), PuzzleColor::B},
                                   {getMinibox(6,12,3), PuzzleColor::A},
                                 };
        board_settings.has_example = true;
        board_settings.example = {{{0,3}, 2}, {{0,8}, 7}, {{0,9}, 2}, {{0,12}, 4}, {{0,14}, 5},
                                  {{1,3}, 3}, {{1,6}, 6}, {{1,8}, 5}, {{1,11}, 9}, {{1,12}, 2}, {{1,13}, 3},
                                  {{2,3}, 1}, {{2,5}, 8}, {{2,9}, 8}, {{2,12}, 1},
                                  {{3,5}, 9}, {{3,13}, 2},
                                  {{4,0}, 6}, {{4,1}, 1}, {{4,2}, 7}, {{4,3}, 8}, {{4,11}, 5}, {{4,12}, 7}, {{4,13}, 1}, {{4,14}, 6},
                                  {{5,1}, 9}, {{5,9}, 9},
                                  {{6,2}, 6}, {{6,5}, 2}, {{6,9}, 4}, {{6,11}, 8},
                                  {{7,1}, 8}, {{7,2}, 9}, {{7,3}, 7}, {{7,6}, 3}, {{7,8}, 6}, {{7,11}, 7},
                                  {{8,0}, 1}, {{8,2}, 2}, {{8,5}, 3}, {{8,6}, 8}, {{8,11}, 2},
                                 };
    }
    else if (type == Type::M9X9_TWICE_COUPLED)
    {
        board_settings.rows = 15;
        board_settings.columns = 15;
        board_settings.values = 9;
        board_settings.min_value = 0;
        board_settings.max_value = 9;
        board_settings.regions = { {getHorizontalRegion(0,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(1,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(2,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(3,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(4,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(5,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(6,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(7,0,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(8,0,9), PuzzleColor::NONE},

                                   {getHorizontalRegion( 6,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion( 7,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion( 8,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion( 9,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(10,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(11,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(12,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(13,6,9), PuzzleColor::NONE},
                                   {getHorizontalRegion(14,6,9), PuzzleColor::NONE},

                                   {getVerticalRegion(0,0,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,1,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,2,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,3,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,4,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,5,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,6,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,7,9), PuzzleColor::NONE},
                                   {getVerticalRegion(0,8,9), PuzzleColor::NONE},

                                   {getVerticalRegion(6, 6,9), PuzzleColor::NONE},
                                   {getVerticalRegion(6, 7,9), PuzzleColor::NONE},
                                   {getVerticalRegion(6, 8,9), PuzzleColor::NONE},
                                   {getVerticalRegion(6, 9,9), PuzzleColor::NONE},
                                   {getVerticalRegion(6,10,9), PuzzleColor::NONE},
                                   {getVerticalRegion(6,11,9), PuzzleColor::NONE},
                                   {getVerticalRegion(6,12,9), PuzzleColor::NONE},
                                   {getVerticalRegion(6,13,9), PuzzleColor::NONE},
                                   {getVerticalRegion(6,14,9), PuzzleColor::NONE},

                                   {getMinibox( 6, 6,3), PuzzleColor::A_DARK},
                                   {getMinibox( 6, 9,3), PuzzleColor::B},
                                   {getMinibox( 6,12,3), PuzzleColor::A},

                                   {getMinibox( 9, 6,3), PuzzleColor::B},
                                   {getMinibox( 9, 9,3), PuzzleColor::A},
                                   {getMinibox( 9,12,3), PuzzleColor::B},

                                   {getMinibox(12, 6,3), PuzzleColor::A},
                                   {getMinibox(12, 9,3), PuzzleColor::B},
                                   {getMinibox(12,12,3), PuzzleColor::A},

                                   { {{6,6}, {7,7}, {8,8}, {9,9}, {10,10}, {11,11}, {12,12}, {13,13}, {14,14}}, PuzzleColor::NONE},
                                   { {{14,6}, {13,7}, {12,8}, {11,9}, {10,10}, {9,11}, {8,12}, {7,13}, {6,14}}, PuzzleColor::NONE},

                                   { {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {1,3}, {2,2}, {2,3}}, PuzzleColor::A},
                                   { {{0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {1,4}, {1,5}, {2,4}, {2,5}}, PuzzleColor::C},
                                   { {{0,8}, {1,6}, {1,7}, {1,8}, {2,6}, {2,7}, {2,8}, {3,7}, {3,8}}, PuzzleColor::B},
                                   { {{2,0}, {2,1}, {3,0}, {3,1}, {3,2}, {3,3}, {4,0}, {4,1}, {4,2}}, PuzzleColor::B},
                                   { {{3,4}, {3,5}, {3,6}, {4,3}, {4,4}, {4,5}, {5,2}, {5,3}, {5,4}}, PuzzleColor::A},
                                   { {{4,6}, {4,7}, {4,8}, {5,5}, {5,6}, {5,7}, {5,8}, {6,5}, {7,5}}, PuzzleColor::C},
                                   { {{5,0}, {5,1}, {6,0}, {6,1}, {6,2}, {7,0}, {7,1}, {7,2}, {8,0}}, PuzzleColor::C},
                                   { {{6,3}, {6,4}, {7,3}, {7,4}, {8,1}, {8,2}, {8,3}, {8,4}, {8,5}}, PuzzleColor::B},
                                 };
        board_settings.empty_points = {{ 9,0}, { 9,1}, { 9,2}, { 9,3}, { 9,4}, { 9,5},
                                       {10,0}, {10,1}, {10,2}, {10,3}, {10,4}, {10,5},
                                       {11,0}, {11,1}, {11,2}, {11,3}, {11,4}, {11,5},
                                       {12,0}, {12,1}, {12,2}, {12,3}, {12,4}, {12,5},
                                       {13,0}, {13,1}, {13,2}, {13,3}, {13,4}, {13,5},
                                       {14,0}, {14,1}, {14,2}, {14,3}, {14,4}, {14,5},
                                       {0,9}, {0,10}, {0,11}, {0,12}, {0,13}, {0,14},
                                       {1,9}, {1,10}, {1,11}, {1,12}, {1,13}, {1,14},
                                       {2,9}, {2,10}, {2,11}, {2,12}, {2,13}, {2,14},
                                       {3,9}, {3,10}, {3,11}, {3,12}, {3,13}, {3,14},
                                       {4,9}, {4,10}, {4,11}, {4,12}, {4,13}, {4,14},
                                       {5,9}, {5,10}, {5,11}, {5,12}, {5,13}, {5,14},
                                      };
        board_settings.has_example = true;
        board_settings.example = {{{0,2}, 1}, {{0,6}, 4},
                                  {{1,3}, 9}, {{1,5}, 5},
                                  {{2,0}, 7}, {{2,4}, 3}, {{2,8}, 4},
                                  {{3,0}, 4}, {{3,4}, 6}, {{3,8}, 7},
                                  {{4,2}, 2}, {{4,6}, 3},
                                  {{5,0}, 6}, {{5,4}, 1}, {{5,8}, 5},
                                  {{6,0}, 1}, {{6,4}, 9}, {{6,12}, 2},
                                  {{7,3}, 7}, {{7,5}, 6}, {{7,9}, 7}, {{7,11}, 3},
                                  {{8,2}, 8}, {{8,10}, 2}, {{8,14}, 4},
                                  {{9,6}, 9}, {{9,10}, 1}, {{9,14}, 6},
                                  {{10,8}, 5}, {{10,12}, 1},
                                  {{11,6}, 7}, {{11,10}, 9}, {{11,14}, 3},
                                  {{12,6}, 2}, {{12,10}, 5}, {{12,14}, 7},
                                  {{13,9}, 2}, {{13,11}, 9},
                                  {{14,8}, 9}, {{14,12}, 6},
                                 };
    }
    return board_settings;
}

}
}

namespace {

void log_var(Minisat::Lit lit) {
    if (sign(lit)) {
        std::clog << '-';
    }
    std::clog << var(lit) + 1 << ' ';
}

void log_clause(Minisat::vec<Minisat::Lit> const& clause) {
    for (int i = 0; i < clause.size(); ++i) {
        log_var(clause[i]);
    }
    std::clog << "0\n";
}

void log_clause(Minisat::Lit lhs, Minisat::Lit rhs) {
    log_var(lhs); log_var(rhs);
    std::clog << "0\n";
}


} //end anonymous namespace

SudokuSolver::SudokuSolver(Settings::Sudoku::BoardSettings board_settings, bool write_dimacs)
    : m_write_dimacs(write_dimacs)
    , m_board_settings(board_settings)
{
    // Initialize the board
    init_variables();
    one_square_one_value();
    non_duplicated_values();
}

Minisat::Var SudokuSolver::toVar(int row, int column, int value)  const{
    assert(row >= 0 && row < m_board_settings.rows && "Attempt to get var for nonexistant row");
    assert(column >= 0 && column < m_board_settings.columns && "Attempt to get var for nonexistant column");
    assert(value >= 0 && value < m_board_settings.values && "Attempt to get var for nonexistant value");
    return row * m_board_settings.columns * m_board_settings.values + column * m_board_settings.values + value;
}

bool SudokuSolver::is_valid(SudokuBoard const& b) const {
    if (b.size() != m_board_settings.rows) {
        return false;
    }
    for (int row = 0; row < m_board_settings.rows; ++row) {
        if (b[row].size() != m_board_settings.columns) {
            return false;
        }
        for (int col = 0; col < m_board_settings.columns; ++col) {
            auto value = b[row][col];
            if (value < m_board_settings.min_value || value > m_board_settings.max_value) {
                return false;
            }
        }
    }
    return true;
}

void SudokuSolver::init_variables() {
    if (m_write_dimacs) {
        std::clog << "c (row, column, value) = variable\n";
    }
    for (int r = 0; r < m_board_settings.rows; ++r) {
        for (int c = 0; c < m_board_settings.columns; ++c) {
            for (int v = 0; v < m_board_settings.values; ++v) {
                auto var = solver.newVar();
                if (m_write_dimacs) {
                    std::clog << "c (" << r << ", " << c << ", " << v + 1 << ") = " << var + 1 << '\n';
                }
            }
        }
    }
    std::clog << std::flush;
}


void SudokuSolver::exactly_one_true(Minisat::vec<Minisat::Lit> const& literals) {
    if (m_write_dimacs) {
        log_clause(literals);
    }
    solver.addClause(literals);
    for (size_t i = 0; i < literals.size(); ++i) {
        for (size_t j = i + 1; j < literals.size(); ++j) {
            if (m_write_dimacs) {
                log_clause(~literals[i], ~literals[j]);
            }
            solver.addClause(~literals[i], ~literals[j]);
        }
    }
}

void SudokuSolver::one_square_one_value() {
    for (int row = 0; row < m_board_settings.rows; ++row) {
        for (int column = 0; column < m_board_settings.columns; ++column) {
            Minisat::vec<Minisat::Lit> literals;
            for (int value = 0; value < m_board_settings.values; ++value) {
                literals.push(Minisat::mkLit(toVar(row, column, value)));
            }
            exactly_one_true(literals);
        }
    }
}

void SudokuSolver::non_duplicated_values() {
    // Allow every value only once in an adjacent region
    for (auto region : m_board_settings.regions)
    {
        for (int value = 0; value < m_board_settings.values; ++value) {
            Minisat::vec<Minisat::Lit> literals;
            for (auto point : region.first) {
                literals.push(Minisat::mkLit(toVar(point.first, point.second, value)));
            }
            exactly_one_true(literals);
        }
    }
}

bool SudokuSolver::apply_board(const SudokuBoard& b) {
    assert(is_valid(b) && "Provided board is not valid!");
    bool ret = true;
    for (int row = 0; row < m_board_settings.rows; ++row) {
        for (int col = 0; col < m_board_settings.columns; ++col) {
            auto value = b[row][col];
            if (value != 0) {
                ret &= solver.addClause(Minisat::mkLit(toVar(row, col, value - 1)));
            }
        }
    }
    return ret;
}

bool SudokuSolver::solve() {
    return solver.solve();
}

SudokuBoard SudokuSolver::get_solution() const {
    SudokuBoard b(m_board_settings.rows, std::vector<int>(m_board_settings.columns));
    for (int row = 0; row < m_board_settings.rows; ++row) {
        for (int col = 0; col < m_board_settings.columns; ++col) {
            if (m_board_settings.empty_points.count(Point(row,col)) == 0)
            {
                int found = 0;
                for (int val = 0; val < m_board_settings.values; ++val) {
                    if (solver.modelValue(toVar(row, col, val)).isTrue()) {
                        ++found;
                        b[row][col] = val + 1;
                    }
                }
                assert(found == 1 && "The SAT solver assigned one position more than one value");
                (void)found;
            }
        }
    }
    return b;
}
