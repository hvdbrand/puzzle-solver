#include "sudokusolver.hpp"

#include <iostream>

namespace Settings {
namespace Sudoku {

namespace {
    PointSet getMinibox(int row, int column, int size)
    {
        PointSet points;
        for (int rr = 0; rr < size; ++rr) {
            for (int cc = 0; cc < size; ++cc) {
                points.emplace(row + rr, column + cc);
            }
        }
        return points;
    }

    PointSet getHorizontalRegion(int row, int column, int size)
    {
        PointSet points;
        for (int cc = 0; cc < size; ++cc) {
            points.emplace(row, column + cc);
        }
        return points;
    }

    PointSet getVerticalRegion(int row, int column, int size)
    {
        PointSet points;
        for (int rr = 0; rr < size; ++rr) {
            points.emplace(row + rr, column);
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
    }
    return board_settings;
}

bool get_sudoku_example(Type sudoku_type, Board& example_board)
{
    bool found_example = false;
    switch (sudoku_type) {
    case Type::S16X16:
        example_board.clear();
        //                                                     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
        example_board.emplace_back(std::initializer_list<int>{ 0,  3, 11,  0,  0,  0, 10, 16,  0,  0,  0,  0,  0,  5,  6,  4}); //  0
        example_board.emplace_back(std::initializer_list<int>{ 0,  0,  0,  0,  3,  0,  0,  0,  0, 11,  6,  0, 14,  0, 15,  1}); //  1
        example_board.emplace_back(std::initializer_list<int>{ 0,  0,  1,  0,  7, 15,  0,  0,  3, 14,  0, 16, 12,  0,  0,  0}); //  2
        example_board.emplace_back(std::initializer_list<int>{ 0, 14,  0,  0,  8, 12,  6,  0,  0,  0, 13,  0, 16,  9,  0,  0}); //  3
        example_board.emplace_back(std::initializer_list<int>{ 0,  4,  0,  0,  0,  0,  8, 12,  0,  0,  0,  7,  0,  3, 14, 15}); //  4
        example_board.emplace_back(std::initializer_list<int>{10,  0,  0, 14, 11,  3,  0,  0,  9,  8,  0,  0,  0,  0,  7,  0}); //  5
        example_board.emplace_back(std::initializer_list<int>{ 3,  2, 12,  7, 16,  9,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0}); //  6
        example_board.emplace_back(std::initializer_list<int>{13,  0,  0,  0, 14,  0,  7,  0,  0,  2,  0,  0,  0,  6,  0,  0}); //  7
        example_board.emplace_back(std::initializer_list<int>{ 0,  0,  8,  0,  0,  0, 14,  0,  0,  7,  0,  4,  0,  0,  0, 13}); //  8
        example_board.emplace_back(std::initializer_list<int>{ 0,  0,  0,  0,  0,  0, 16,  0,  0,  0, 15,  8,  4, 11,  3,  2}); //  9
        example_board.emplace_back(std::initializer_list<int>{ 0, 16,  0,  0,  0,  0, 13,  8,  0,  0,  5, 14, 15,  0,  0,  7}); // 10
        example_board.emplace_back(std::initializer_list<int>{12,  9,  3,  0,  4,  0,  0,  0,  2, 13,  0,  0,  0,  0,  5,  0}); // 11
        example_board.emplace_back(std::initializer_list<int>{ 0,  0, 13, 10,  0,  8,  0,  0,  0, 12,  7, 11,  0,  0, 16,  0}); // 12
        example_board.emplace_back(std::initializer_list<int>{ 0,  0,  0,  1,  9,  0,  5, 11,  0,  0, 14,  6,  0,  7,  0,  0}); // 13
        example_board.emplace_back(std::initializer_list<int>{ 5,  7,  0,  8,  0,  4, 12,  0,  0,  0,  0,  2,  0,  0,  0,  0}); // 14
        example_board.emplace_back(std::initializer_list<int>{ 4, 11, 14,  0,  0,  0,  0,  0,  8, 15,  0,  0,  0, 13, 10,  0}); // 15
        found_example = true;
        break;
    case Type::T9X15:
        example_board.clear();
        //                                                    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 2, 0, 0, 0, 0, 7, 2, 0, 0, 4, 0, 5}); // 0
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 3, 0, 0, 6, 0, 5, 0, 0, 9, 2, 3, 0}); // 1
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 1, 0, 8, 0, 0, 0, 8, 0, 0, 1, 0, 0}); // 2
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 2, 0}); // 3
        example_board.emplace_back(std::initializer_list<int>{6, 1, 7, 8, 0, 0, 0, 0, 0, 0, 0, 5, 7, 1, 6}); // 4
        example_board.emplace_back(std::initializer_list<int>{0, 9, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0}); // 5
        example_board.emplace_back(std::initializer_list<int>{0, 0, 6, 0, 0, 2, 0, 0, 0, 4, 0, 8, 0, 0, 0}); // 6
        example_board.emplace_back(std::initializer_list<int>{0, 8, 9, 7, 0, 0, 3, 0, 6, 0, 0, 7, 0, 0, 0}); // 7
        example_board.emplace_back(std::initializer_list<int>{1, 0, 2, 0, 0, 3, 8, 0, 0, 0, 0, 2, 0, 0, 0}); // 8
        found_example = true;
        break;
    case Type::M9X9_TWICE_COUPLED:
        example_board.clear();
        //                                                    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
        example_board.emplace_back(std::initializer_list<int>{0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0}); // 0
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 9, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0}); // 1
        example_board.emplace_back(std::initializer_list<int>{7, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0}); // 2
        example_board.emplace_back(std::initializer_list<int>{4, 0, 0, 0, 6, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0}); // 3
        example_board.emplace_back(std::initializer_list<int>{0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0}); // 4
        example_board.emplace_back(std::initializer_list<int>{6, 0, 0, 0, 1, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0}); // 5
        example_board.emplace_back(std::initializer_list<int>{1, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0}); // 6
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 7, 0, 6, 0, 0, 0, 7, 0, 3, 0, 0, 0}); // 7
        example_board.emplace_back(std::initializer_list<int>{0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 4}); // 8
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 1, 0, 0, 0, 6}); // 9
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 1, 0, 0}); // 10
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 9, 0, 0, 0, 3}); // 11
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 5, 0, 0, 0, 7}); // 12
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 9, 0, 0, 0}); // 13
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 6, 0, 0}); // 14
        found_example = true;
        break;

    default:
        found_example = false;
        break;
    }
    return found_example;
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

bool SudokuSolver::is_valid(Board const& b) const {
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
            //if (m_board_settings.empty_points.)
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

void SudokuSolver::empty_points()
{
    for (auto point : m_board_settings.empty_points) {

    }
}

bool SudokuSolver::apply_board(Board const& b) {
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

Board SudokuSolver::get_solution() const {
    Board b(m_board_settings.rows, std::vector<int>(m_board_settings.columns));
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
