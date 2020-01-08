#include "solver.hpp"

#include <iostream>


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

Solver::Solver(Settings::Sudoku::BoardSettings board_settings, bool write_dimacs)
    : m_write_dimacs(write_dimacs)
    , m_board_settings(board_settings)
{
    // Initialize the board
    init_variables();
    one_square_one_value();
    non_duplicated_values();
}

Minisat::Var Solver::toVar(int row, int column, int value)  const{
    assert(row >= 0 && row < m_board_settings.rows && "Attempt to get var for nonexistant row");
    assert(column >= 0 && column < m_board_settings.columns && "Attempt to get var for nonexistant column");
    assert(value >= 0 && value < m_board_settings.values && "Attempt to get var for nonexistant value");
    return row * m_board_settings.columns * m_board_settings.values + column * m_board_settings.values + value;
}

bool Solver::is_valid(Board const& b) const {
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

void Solver::init_variables() {
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


void Solver::exactly_one_true(Minisat::vec<Minisat::Lit> const& literals) {
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

void Solver::one_square_one_value() {
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

void Solver::non_duplicated_values() {
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

bool Solver::apply_board(Board const& b) {
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

bool Solver::solve() {
    return solver.solve();
}

Board Solver::get_solution() const {
    Board b(m_board_settings.rows, std::vector<int>(m_board_settings.columns));
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
