#include "sudokusolver.hpp"

#include <iostream>

namespace Settings {
namespace Sudoku {

BoardSettings get_board_settings(Type type)
{
    BoardSettings board_settings;
    if (type == Type::S9X9)
    {
        board_settings.rows = 9;
        board_settings.columns = 9;
        board_settings.values = 9;
        board_settings.min_value = 0;
        board_settings.max_value = 9;
        board_settings.minibox_size = 3;
        board_settings.horizontal_regions = { {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0}};
    }
    else if (type == Type::S16X16)
    {
        board_settings.rows = 16;
        board_settings.columns = 16;
        board_settings.values = 16;
        board_settings.min_value = 0;
        board_settings.max_value = 16;
        board_settings.minibox_size = 4;
        board_settings.horizontal_regions = { {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0}, {9,0}, {10,0}, {11,0}, {12,0}, {13,0}, {14,0}, {15,0}};
    }
    else if (type == Type::T9x15)
    {
        board_settings.rows = 9;
        board_settings.columns = 15;
        board_settings.values = 9;
        board_settings.min_value = 0;
        board_settings.max_value = 9;
        board_settings.minibox_size = 3;
        board_settings.horizontal_regions = { {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0},
                                              {0,6}, {1,6}, {2,6}, {3,6}, {4,6}, {5,6}, {6,6}, {7,6}, {8,6}};
    }
    return board_settings;
}

bool get_sudoku_example(Type sudoku_type, SudokuBoard& example_board)
{
    bool found_example = false;
    switch (sudoku_type) {
    case Type::S9X9:
        example_board.clear();
        //                                                     1  2  3  4  5  6  7  8  9
        example_board.emplace_back(std::initializer_list<int>{0, 2, 6, 0, 0, 0, 8, 1, 0}); // 1
        example_board.emplace_back(std::initializer_list<int>{3, 0, 0, 7, 0, 8, 0, 0, 6}); // 2
        example_board.emplace_back(std::initializer_list<int>{4, 0, 0, 0, 5, 0, 0, 0, 7}); // 3
        example_board.emplace_back(std::initializer_list<int>{0, 5, 0, 1, 0, 7, 0, 9, 0}); // 4
        example_board.emplace_back(std::initializer_list<int>{0, 0, 3, 9, 0, 5, 1, 0, 0}); // 5
        example_board.emplace_back(std::initializer_list<int>{0, 4, 0, 3, 0, 2, 0, 5, 0}); // 6
        example_board.emplace_back(std::initializer_list<int>{1, 0, 0, 0, 3, 0, 0, 0, 2}); // 7
        example_board.emplace_back(std::initializer_list<int>{5, 0, 0, 2, 0, 4, 0, 0, 9}); // 8
        example_board.emplace_back(std::initializer_list<int>{0, 3, 8, 0, 0, 0, 4, 6, 0}); // 9
        found_example = true;
        break;
    case Type::S16X16:
        example_board.clear();
        //                                                     1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        example_board.emplace_back(std::initializer_list<int>{ 0,  3, 11,  0,  0,  0, 10, 16,  0,  0,  0,  0,  0,  5,  6,  4}); //  1
        example_board.emplace_back(std::initializer_list<int>{ 0,  0,  0,  0,  3,  0,  0,  0,  0, 11,  6,  0, 14,  0, 15,  1}); //  2
        example_board.emplace_back(std::initializer_list<int>{ 0,  0,  1,  0,  7, 15,  0,  0,  3, 14,  0, 16, 12,  0,  0,  0}); //  3
        example_board.emplace_back(std::initializer_list<int>{ 0, 14,  0,  0,  8, 12,  6,  0,  0,  0, 13,  0, 16,  9,  0,  0}); //  4
        example_board.emplace_back(std::initializer_list<int>{ 0,  4,  0,  0,  0,  0,  8, 12,  0,  0,  0,  7,  0,  3, 14, 15}); //  5
        example_board.emplace_back(std::initializer_list<int>{10,  0,  0, 14, 11,  3,  0,  0,  9,  8,  0,  0,  0,  0,  7,  0}); //  6
        example_board.emplace_back(std::initializer_list<int>{ 3,  2, 12,  7, 16,  9,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0}); //  7
        example_board.emplace_back(std::initializer_list<int>{13,  0,  0,  0, 14,  0,  7,  0,  0,  2,  0,  0,  0,  6,  0,  0}); //  8
        example_board.emplace_back(std::initializer_list<int>{ 0,  0,  8,  0,  0,  0, 14,  0,  0,  7,  0,  4,  0,  0,  0, 13}); //  9
        example_board.emplace_back(std::initializer_list<int>{ 0,  0,  0,  0,  0,  0, 16,  0,  0,  0, 15,  8,  4, 11,  3,  2}); // 10
        example_board.emplace_back(std::initializer_list<int>{ 0, 16,  0,  0,  0,  0, 13,  8,  0,  0,  5, 14, 15,  0,  0,  7}); // 11
        example_board.emplace_back(std::initializer_list<int>{12,  9,  3,  0,  4,  0,  0,  0,  2, 13,  0,  0,  0,  0,  5,  0}); // 12
        example_board.emplace_back(std::initializer_list<int>{ 0,  0, 13, 10,  0,  8,  0,  0,  0, 12,  7, 11,  0,  0, 16,  0}); // 13
        example_board.emplace_back(std::initializer_list<int>{ 0,  0,  0,  1,  9,  0,  5, 11,  0,  0, 14,  6,  0,  7,  0,  0}); // 14
        example_board.emplace_back(std::initializer_list<int>{ 5,  7,  0,  8,  0,  4, 12,  0,  0,  0,  0,  2,  0,  0,  0,  0}); // 15
        example_board.emplace_back(std::initializer_list<int>{ 4, 11, 14,  0,  0,  0,  0,  0,  8, 15,  0,  0,  0, 13, 10,  0}); // 16
        found_example = true;
        break;
    case Type::T9x15:
        example_board.clear();
        //                                                     1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 2, 0, 0, 0, 0, 7, 2, 0, 0, 4, 0, 5}); // 1
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 3, 0, 0, 6, 0, 5, 0, 0, 9, 2, 3, 0}); // 2
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 1, 0, 8, 0, 0, 0, 8, 0, 0, 1, 0, 0}); // 3
        example_board.emplace_back(std::initializer_list<int>{0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 2, 0}); // 4
        example_board.emplace_back(std::initializer_list<int>{6, 1, 7, 8, 0, 0, 0, 0, 0, 0, 0, 5, 7, 1, 6}); // 5
        example_board.emplace_back(std::initializer_list<int>{0, 9, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0}); // 6
        example_board.emplace_back(std::initializer_list<int>{0, 0, 6, 0, 0, 2, 0, 0, 0, 4, 0, 8, 0, 0, 0}); // 7
        example_board.emplace_back(std::initializer_list<int>{0, 8, 9, 7, 0, 0, 3, 0, 6, 0, 0, 7, 0, 0, 0}); // 8
        example_board.emplace_back(std::initializer_list<int>{1, 0, 2, 0, 0, 3, 8, 0, 0, 0, 0, 2, 0, 0, 0}); // 9
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
    // Allow every value only once in a horizontal region
    for(auto region_start : m_board_settings.horizontal_regions)
    {
        auto start_row = region_start.first;
        auto start_column = region_start.second;
        for (int value = 0; value < m_board_settings.values; ++value) {
            Minisat::vec<Minisat::Lit> literals;
            for (int column = 0; column < m_board_settings.values; ++column) {
                literals.push(Minisat::mkLit(toVar(start_row, start_column + column, value)));
            }
            exactly_one_true(literals);
        }
    }

    // In each column, for each value, forbid two rows sharing that value
    for (int column = 0; column < m_board_settings.columns; ++column) {
        for (int value = 0; value < m_board_settings.values; ++value) {
            Minisat::vec<Minisat::Lit> literals;
            for (int row = 0; row < m_board_settings.rows; ++row) {
                literals.push(Minisat::mkLit(toVar(row, column, value)));
            }
            exactly_one_true(literals);
        }
    }

    // Now forbid sharing in the miniboxes (3x3 or 4x4 boxes)
    for (int r = 0; r < m_board_settings.rows; r += m_board_settings.minibox_size) {
        for (int c = 0; c < m_board_settings.columns; c += m_board_settings.minibox_size) {
            for (int value = 0; value < m_board_settings.values; ++value) {
                Minisat::vec<Minisat::Lit> literals;
                for (int rr = 0; rr < m_board_settings.minibox_size; ++rr) {
                    for (int cc = 0; cc < m_board_settings.minibox_size; ++cc) {
                        literals.push(Minisat::mkLit(toVar(r + rr, c + cc, value)));
                    }
                }
                exactly_one_true(literals);
            }
        }
    }
}

bool SudokuSolver::apply_board(SudokuBoard const& b) {
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
    return b;
}
