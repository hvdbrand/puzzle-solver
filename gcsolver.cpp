#include "gcsolver.hpp"

#include <iostream>

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

const int UNSPECIFIED_ADJACENTS = 5;
} //end anonymous namespace

GcSolver::GcSolver(GcSolverSettings solver_settings, bool write_dimacs)
    : m_solver_settings(solver_settings)
    , m_write_dimacs(write_dimacs)
{
    m_adjacents.resize(m_solver_settings.vertical_cell_count, std::vector<int>(m_solver_settings.horizontal_cell_count, UNSPECIFIED_ADJACENTS));
    m_adjacents[0][3] = 1;
    m_adjacents[0][4] = 1;
    m_adjacents[0][5] = 1;
    m_adjacents[0][6] = 1;
    m_adjacents[0][7] = 3;
    m_adjacents[0][8] = 2;
    m_adjacents[0][9] = 1;
    m_adjacents[1][1] = 1;
    m_adjacents[1][2] = 2;
    m_adjacents[1][3] = 2;
    m_adjacents[1][4] = 1;
    m_adjacents[1][5] = 2;
    m_adjacents[1][6] = 2;
    m_adjacents[1][7] = 1;
    m_adjacents[1][8] = 1;
    m_adjacents[1][9] = 2;
    m_adjacents[2][0] = 1;
    m_adjacents[2][1] = 2;
    m_adjacents[2][2] = 1;
    m_adjacents[2][5] = 1;
    m_adjacents[2][6] = 2;
    m_adjacents[2][7] = 1;
    m_adjacents[2][8] = 1;
    m_adjacents[2][9] = 2;
    m_adjacents[3][0] = 1;
    m_adjacents[3][1] = 2;
    m_adjacents[3][4] = 1;
    m_adjacents[3][5] = 2;
    m_adjacents[3][6] = 2;
    m_adjacents[3][7] = 1;
    m_adjacents[3][8] = 1;
    m_adjacents[3][9] = 3;
    m_adjacents[4][0] = 1;
    m_adjacents[4][1] = 3;
    m_adjacents[4][2] = 1;
    m_adjacents[4][4] = 2;
    m_adjacents[4][5] = 2;
    m_adjacents[4][6] = 1;
    m_adjacents[4][7] = 1;
    m_adjacents[4][9] = 2;
    m_adjacents[5][0] = 3;
    m_adjacents[5][1] = 2;
    m_adjacents[5][2] = 1;
    m_adjacents[5][3] = 1;
    m_adjacents[5][4] = 2;
    m_adjacents[5][6] = 1;
    m_adjacents[5][7] = 1;
    m_adjacents[5][9] = 2;
    m_adjacents[6][0] = 1;
    m_adjacents[6][1] = 1;
    m_adjacents[6][2] = 3;
    m_adjacents[6][3] = 2;
    m_adjacents[6][4] = 2;
    m_adjacents[6][5] = 1;
    m_adjacents[6][6] = 2;
    m_adjacents[6][7] = 1;
    m_adjacents[6][8] = 1;
    m_adjacents[6][9] = 2;
    m_adjacents[7][1] = 1;
    m_adjacents[7][2] = 2;
    m_adjacents[7][3] = 1;
    m_adjacents[7][4] = 1;
    m_adjacents[7][5] = 1;
    m_adjacents[7][6] = 2;
    m_adjacents[7][8] = 2;
    m_adjacents[7][9] = 1;
    m_adjacents[8][1] = 2;
    m_adjacents[8][2] = 1;
    m_adjacents[8][3] = 2;
    m_adjacents[8][5] = 1;
    m_adjacents[8][6] = 3;
    m_adjacents[8][7] = 2;
    m_adjacents[8][8] = 2;
    m_adjacents[8][9] = 2;
    m_adjacents[9][0] = 1;
    m_adjacents[9][1] = 2;
    m_adjacents[9][5] = 2;
    m_adjacents[9][6] = 2;
    m_adjacents[9][7] = 3;
    m_adjacents[9][8] = 1;
    m_adjacents[9][9] = 3;
    m_adjacents[10][0] = 1;
    m_adjacents[10][1] = 2;
    m_adjacents[10][2] = 1;
    m_adjacents[10][3] = 1;
    m_adjacents[10][4] = 2;
    m_adjacents[10][5] = 2;
    m_adjacents[10][7] = 1;

    init_variables();
    add_clauses_for_segments_surrounding_cells();
    add_clauses_for_corners();
    // TODO: Add clauses to enforce a single closed structur
    // An intermediate step might be enforcing 0 or 2 lines on every crossing
}

Minisat::Var GcSolver::toVar(int line, int segment, bool horizontal)  const{
    const int line_count = horizontal ? m_solver_settings.horizontal_line_count : m_solver_settings.vertical_line_count;
    const int segment_count = horizontal ? m_solver_settings.horizontal_cell_count : m_solver_settings.vertical_cell_count;
    assert((line >= 0) && (line < line_count) && "Attempt to get var for nonexistant row");
    assert((segment >= 0) && (segment < segment_count) && "Attempt to get var for nonexistant segment");
    const int vertical_offset = m_solver_settings.horizontal_cell_count * m_solver_settings.horizontal_line_count;
    return line * segment_count + segment + (horizontal ? 0 : vertical_offset);
}

bool GcSolver::is_valid(const GcBoard& b) const {
    bool is_valid = true;
    if ((b.horizontal_lines.size() == m_solver_settings.horizontal_line_count) &&
        (b.vertical_lines.size() && m_solver_settings.vertical_line_count))
    {
        for (const auto& line : b.horizontal_lines)
        {
            if (line.size() != m_solver_settings.horizontal_cell_count)
            {
                is_valid = false;
                break;
            }
        }
        if (is_valid)
        {
            for (const auto& line : b.vertical_lines)
            {
                if (line.size() != m_solver_settings.vertical_cell_count)
                {
                    is_valid = false;
                    break;
                }
            }
        }
    }
    else
    {
        is_valid = false;
    }
    return true;
}

void GcSolver::init_variables() {
    if (m_write_dimacs) {
        std::clog << "horizontal\nc (line, segment) = variable\n";
    }
    for (int l = 0; l < m_solver_settings.horizontal_line_count; ++l) {
        for (int s = 0; s < m_solver_settings.horizontal_cell_count; ++s) {
            auto var = m_solver.newVar();
            if (m_write_dimacs) {
                std::clog << "c (" << l << ", " << s << ") = " << var + 1 << '\n';
            }
        }
    }

    if (m_write_dimacs) {
        std::clog << "vertical\nc (line, segment) = variable\n";
    }
    for (int l = 0; l < m_solver_settings.vertical_line_count; ++l) {
        for (int s = 0; s < m_solver_settings.vertical_cell_count; ++s) {
            auto var = m_solver.newVar();
            if (m_write_dimacs) {
                std::clog << "c (" << l << ", " << s << ") = " << var + 1 << '\n';
            }
        }
    }

    std::clog << std::flush;
}

void GcSolver::add_clauses_for_segments_surrounding_cells()
{
    for (int row = 0; row < m_solver_settings.vertical_cell_count; ++row)
    {
        for (int col = 0; col< m_solver_settings.horizontal_cell_count; ++col)
        {
            if (m_adjacents[row][col] != UNSPECIFIED_ADJACENTS)
            {
                Minisat::vec<Minisat::Lit> literals;
                literals.push(Minisat::mkLit(toVar(row, col, true)));
                literals.push(Minisat::mkLit(toVar(row + 1, col, true)));
                literals.push(Minisat::mkLit(toVar(col, row, false)));
                literals.push(Minisat::mkLit(toVar(col + 1, row, false)));
                switch(m_adjacents[row][col])
                {
                case 0:
                    none_true(literals);
                    break;
                case 1:
                    exactly_one_true(literals);
                    break;
                case 2:
                    exactly_two_true(literals);
                    break;
                case 3:
                    exactly_three_true(literals);
                    break;
                }
            }
        }
    }
}

void GcSolver::add_clauses_for_corners()
{
    Minisat::vec<Minisat::Lit> literals;
    literals.push(Minisat::mkLit(toVar(0, 0, true)));
    literals.push(Minisat::mkLit(toVar(0, 0, false)));
    none_or_two(literals);
    literals.clear();
    literals.push(Minisat::mkLit(toVar(0, m_solver_settings.horizontal_cell_count - 1, true)));
    literals.push(Minisat::mkLit(toVar(m_solver_settings.vertical_line_count - 1, 0, false)));
    none_or_two(literals);
    literals.clear();
    literals.push(Minisat::mkLit(toVar(m_solver_settings.horizontal_line_count - 1, 0, true)));
    literals.push(Minisat::mkLit(toVar(0, m_solver_settings.vertical_cell_count - 1, false)));
    none_or_two(literals);
    literals.clear();
    literals.push(Minisat::mkLit(toVar(m_solver_settings.horizontal_line_count - 1, m_solver_settings.horizontal_cell_count - 1, true)));
    literals.push(Minisat::mkLit(toVar(m_solver_settings.vertical_line_count - 1, m_solver_settings.vertical_cell_count - 1, false)));
    none_or_two(literals);
    for(size_t col=1; col < m_solver_settings.horizontal_cell_count; ++col)
    {
        literals.clear();
        literals.push(Minisat::mkLit(toVar(0, col - 1, true)));
        literals.push(Minisat::mkLit(toVar(0, col, true)));
        literals.push(Minisat::mkLit(toVar(col, 0, false)));
        none_or_two(literals);
        literals.clear();
        literals.push(Minisat::mkLit(toVar(m_solver_settings.horizontal_line_count - 1, col - 1, true)));
        literals.push(Minisat::mkLit(toVar(m_solver_settings.horizontal_line_count - 1, col, true)));
        literals.push(Minisat::mkLit(toVar(col, m_solver_settings.vertical_cell_count - 1, false)));
        none_or_two(literals);
    }
    for(size_t row=1; row < m_solver_settings.vertical_cell_count; ++row)
    {
        literals.clear();
        literals.push(Minisat::mkLit(toVar(0, row - 1, false)));
        literals.push(Minisat::mkLit(toVar(0, row, false)));
        literals.push(Minisat::mkLit(toVar(row, 0, true)));
        none_or_two(literals);
        literals.clear();
        literals.push(Minisat::mkLit(toVar(m_solver_settings.vertical_line_count - 1, row - 1, false)));
        literals.push(Minisat::mkLit(toVar(m_solver_settings.vertical_line_count - 1, row, false)));
        literals.push(Minisat::mkLit(toVar(row, m_solver_settings.horizontal_cell_count - 1, true)));
        none_or_two(literals);
    }
    for (size_t row=1; row < m_solver_settings.vertical_cell_count; ++row)
    {
        for(size_t col=1; col < m_solver_settings.horizontal_cell_count; ++col)
        {
            literals.clear();
            literals.push(Minisat::mkLit(toVar(row, col - 1, true)));
            literals.push(Minisat::mkLit(toVar(row, col, true)));
            literals.push(Minisat::mkLit(toVar(col, row - 1, false)));
            literals.push(Minisat::mkLit(toVar(col, row, false)));
            //none_or_two(literals);
        }
    }
}

void GcSolver::none_or_two(Minisat::vec<Minisat::Lit> const& literals) {
    if (literals.size() == 2)
    {
        if (m_write_dimacs) {
            log_clause( literals[0], ~literals[1]);
            log_clause(~literals[0],  literals[1]);
        }
        m_solver.addClause( literals[0], ~literals[1]);
        m_solver.addClause(~literals[0],  literals[1]);
    }
    else if (literals.size() == 3)
    {
        Minisat::vec<Minisat::Lit> inv_literals;
        inv_literals.push(~literals[0]);
        inv_literals.push(~literals[1]);
        inv_literals.push(~literals[2]);
        if (m_write_dimacs) {
            log_clause(inv_literals);
        }
        m_solver.addClause(inv_literals);
        Minisat::vec<Minisat::Lit> current_literals;
        current_literals.push(~literals[0]);
        current_literals.push( literals[1]);
        current_literals.push( literals[2]);
        if (m_write_dimacs) {
            log_clause(current_literals);
        }
        m_solver.addClause(current_literals);
        current_literals.clear();
        current_literals.push( literals[0]);
        current_literals.push(~literals[1]);
        current_literals.push( literals[2]);
        if (m_write_dimacs) {
            log_clause(current_literals);
        }
        m_solver.addClause(current_literals);
        current_literals.clear();
        current_literals.push( literals[0]);
        current_literals.push( literals[1]);
        current_literals.push(~literals[2]);
        if (m_write_dimacs) {
            log_clause(current_literals);
        }
        m_solver.addClause(current_literals);
    }
    else if (literals.size() == 4)
    {
        Minisat::vec<Minisat::Lit> inv_literals;
        for (size_t i=0; i<literals.size(); ++i)
        {
            inv_literals.push(~literals[i]);
        }
        if (m_write_dimacs) {
            log_clause(inv_literals);
        }
        m_solver.addClause(inv_literals);
        for (size_t i=0; i<literals.size(); ++i)
        {
            Minisat::vec<Minisat::Lit> current_literals;
            Minisat::vec<Minisat::Lit> current_inv_literals;
            for (size_t j=0; j<literals.size(); ++j)
            {
                if (j == i)
                {
                    current_literals.push(inv_literals[j]);
                    current_inv_literals.push(literals[j]);
                }
                else
                {
                    current_literals.push(literals[j]);
                    current_inv_literals.push(inv_literals[j]);
                }
            }
            if (m_write_dimacs) {
                log_clause(current_literals);
                log_clause(current_inv_literals);
            }
            m_solver.addClause(current_literals);
            m_solver.addClause(current_inv_literals);
        }
    }
}

void GcSolver::none_true(Minisat::vec<Minisat::Lit> const& literals) {
    for (size_t i = 0; i < literals.size(); ++i) {
        if (m_write_dimacs) {
            log_var(~literals[i]);
        }
        m_solver.addClause(~literals[i]);
    }
}

void GcSolver::exactly_one_true(Minisat::vec<Minisat::Lit> const& literals) {
    if (m_write_dimacs) {
        log_clause(literals);
    }
    m_solver.addClause(literals);
    for (size_t i = 0; i < literals.size(); ++i) {
        for (size_t j = i + 1; j < literals.size(); ++j) {
            if (m_write_dimacs) {
                log_clause(~literals[i], ~literals[j]);
            }
            m_solver.addClause(~literals[i], ~literals[j]);
        }
    }
}

void GcSolver::exactly_two_true(Minisat::vec<Minisat::Lit> const& literals) {
    Minisat::vec<Minisat::Lit> inv_literals;
    for (auto lit : literals) {
        inv_literals.push(~lit);
    }

    for (size_t i = 0; i < literals.size(); ++i) {
        Minisat::vec<Minisat::Lit> current_inv_literals;
        Minisat::vec<Minisat::Lit> current_literals;
        for (size_t j = 0; j < literals.size(); ++j) {
            if (j != i)
            {
                current_literals.push(literals[j]);
                current_inv_literals.push(inv_literals[j]);
            }
        }
        if (m_write_dimacs) {

            log_clause(current_literals);
            log_clause(current_inv_literals);
        }
        m_solver.addClause(current_literals);
        m_solver.addClause(current_inv_literals);
    }
}

void GcSolver::exactly_three_true(Minisat::vec<Minisat::Lit> const& literals) {

    Minisat::vec<Minisat::Lit> inv_literals;
    for (auto lit : literals) {
        inv_literals.push(~lit);
    }

    if (m_write_dimacs) {
        log_clause(inv_literals);
    }
    m_solver.addClause(inv_literals);

    for (size_t i = 0; i < literals.size(); ++i) {
        for (size_t j = i + 1; j < literals.size(); ++j) {
            if (m_write_dimacs) {
                log_clause(literals[i], literals[j]);
            }
            m_solver.addClause(literals[i], literals[j]);
        }
    }
}

bool GcSolver::apply_board(const GcBoard& b) {
    assert(is_valid(b) && "Provided board is not valid!");
    bool ret = true;
    bool is_horizontal = true;
    for (int l = 0; l < m_solver_settings.horizontal_line_count; ++l) {
        for (int s = 0; s < m_solver_settings.horizontal_cell_count; ++s) {
            if (b.horizontal_lines[l][s])
            {
                ret &= m_solver.addClause(Minisat::mkLit(toVar(l, s, is_horizontal)));
            }
        }
    }
    is_horizontal = false;
    for (int l = 0; l < m_solver_settings.vertical_line_count; ++l) {
        for (int s = 0; s < m_solver_settings.vertical_cell_count; ++s) {
            if (b.vertical_lines[l][s])
            {
                ret &= m_solver.addClause(Minisat::mkLit(toVar(l, s, is_horizontal)));
            }
        }
    }
    return ret;
}

bool GcSolver::solve() {
    return m_solver.solve();
}

GcBoard GcSolver::get_solution() const {
    GcBoard b;
    b.horizontal_lines.resize(m_solver_settings.horizontal_line_count, std::vector<bool>(m_solver_settings.horizontal_cell_count, false));
    b.vertical_lines.resize(m_solver_settings.vertical_line_count, std::vector<bool>(m_solver_settings.vertical_cell_count, false));

    bool is_horizontal = true;
    for (int l = 0; l < m_solver_settings.horizontal_line_count; ++l) {
        for (int s = 0; s < m_solver_settings.horizontal_cell_count; ++s) {
            b.horizontal_lines[l][s] = m_solver.modelValue(toVar(l, s, is_horizontal)).isTrue();
        }
    }
    is_horizontal = false;
    for (int l = 0; l < m_solver_settings.vertical_line_count; ++l) {
        for (int s = 0; s < m_solver_settings.vertical_cell_count; ++s) {
            b.vertical_lines[l][s] = m_solver.modelValue(toVar(l, s, is_horizontal)).isTrue();
        }
    }
    return b;
}
