#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <chrono>
#include <iostream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    DROPDOWN_TO_PUZZLETYPE {{"9x9 Sudoku", Settings::PuzzleType::SUDOKU_9X9},
                            {"16x16 Sudoku", Settings::PuzzleType::SUDOKU_16X16},
                            {"9x15 Twin Sudoku", Settings::PuzzleType::TWINSUDOKU_9X15}},
    m_puzzle(Settings::PuzzleType::SUDOKU_9X9)

{
    ui->setupUi(this);
    change_board_type(Settings::PuzzleType::SUDOKU_9X9);
    for (auto puzzle_type : DROPDOWN_TO_PUZZLETYPE)
    {
        ui->comboBox->addItem(QString::fromStdString(puzzle_type.first));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::change_board_type(Settings::PuzzleType new_puzzle_type)
{
    m_puzzle = Puzzle(new_puzzle_type);

    ui->tableWidget->setRowCount(m_puzzle.getRows());
    ui->tableWidget->setColumnCount(m_puzzle.getColumns());
    ui->tableWidget->clearContents();
    ui->tableWidget->clearFocus();
    int width = 30;
    for (int column = 0; column < m_puzzle.getColumns(); ++column)
    {
        ui->tableWidget->setColumnWidth(column, width);
    }

    QFont font("Helvetica", 12);
    ui->tableWidget->setFont(font);
    for (int row = 0; row < m_puzzle.getRows(); ++ row)
    {
        for (int column = 0; column < m_puzzle.getColumns(); ++column)
        {
            ui->tableWidget->setItem(row, column, new QTableWidgetItem);
            ui->tableWidget->item(row, column)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
        }
    }

    color_miniboxes();
}

void MainWindow::color_miniboxes()
{
    QColor baseColor1(255,0,0,50);
    QColor baseColor2(0,0,255,50);
    bool start_row_with_colored_minibox = true;
    for (int row = 0; row < m_puzzle.getRows(); row += m_puzzle.getMiniboxSize())
    {
        bool color_minibox = start_row_with_colored_minibox;
        for (int column = 0; column < m_puzzle.getColumns(); column += m_puzzle.getMiniboxSize())
        {
            QColor color = color_minibox ? baseColor1 : baseColor2;
            for (int minibox_row = 0; minibox_row < m_puzzle.getMiniboxSize(); ++minibox_row)
            {
                for (int minibox_column = 0; minibox_column < m_puzzle.getMiniboxSize(); ++minibox_column)
                {
                    ui->tableWidget->item(row + minibox_row, column + minibox_column)->setBackgroundColor(color);
                }
            }
            color_minibox = !color_minibox;
        }
        start_row_with_colored_minibox = !start_row_with_colored_minibox;
    }
    // For the twin sudoku, recolor the middle region
    if (m_puzzle.isTwinSudoku())
    {
        QColor middleRegionColor1(170,20,20,120);
        QColor middleRegionColor2(20,20,170,120);
        int first_overlap_column = m_puzzle.getRows() - m_puzzle.getMiniboxSize();
        int first_nonoverlap_column = m_puzzle.getRows();
        for (int row = 0; row < m_puzzle.getRows(); ++row)
        {
            for (int column = first_overlap_column; column < first_nonoverlap_column; ++column)
            {
                auto item = ui->tableWidget->item(row, column);
                if (item->backgroundColor() == baseColor1)
                {
                    item->setBackgroundColor(middleRegionColor1);
                }
                else
                {
                    item->setBackgroundColor(middleRegionColor2);
                }
            }
        }
    }
}

Board MainWindow::parse_board()
{
    Board parsed(m_puzzle.getRows(), std::vector<int>(m_puzzle.getColumns()));
    QFont font("Helvetica", 14, QFont::Bold);
    for (int row = 0; row < m_puzzle.getRows(); ++row)
    {
        for (int column = 0; column < m_puzzle.getColumns(); ++column)
        {
            QTableWidgetItem* item = ui->tableWidget->item(row, column);
            if (item != nullptr)
            {
                parsed[row][column] = item->text().toInt();
                ui->tableWidget->item(row, column)->setFont(font);
            }
        }
    }
    return parsed;
}

void MainWindow::set_board(Board board_to_set)
{
    QFont font("Helvetica", 12);
    for (int row = 0; row < m_puzzle.getRows(); ++row)
    {
        for (int column = 0; column < m_puzzle.getColumns(); ++column)
        {
            QTableWidgetItem* item = ui->tableWidget->item(row, column);
            bool board_cell_is_not_empty = board_to_set[row][column] != 0;
            bool tableitem_is_empty = item->text().length() == 0;
            if (board_cell_is_not_empty && tableitem_is_empty)
            {
                ui->tableWidget->item(row, column)->setText(QString::number(board_to_set[row][column]));
                ui->tableWidget->item(row, column)->setFont(font);
            }
        }
    }
}

bool MainWindow::solve(Board &board_to_solve)
{
    bool solved = false;
    auto t1 = std::chrono::high_resolution_clock::now();
    if (!m_puzzle.apply_board(board_to_solve)) {
        ui->output->append("There is a contradiction in the parsed board!");
        solved = false;
    }
    if (m_puzzle.solve()) {
        std::chrono::duration<double, std::milli> time_taken = std::chrono::high_resolution_clock::now() - t1;
        std::ostringstream ss;
        ss << "Solution found in " << time_taken.count() << " ms";
        ui->output->append(QString::fromStdString(ss.str()));
        board_to_solve = m_puzzle.get_solution();
        solved = true;
    } else {
        std::ostringstream ss;
        ss << "Solving the provided parsed is not possible!";
        ui->output->append(QString::fromStdString(ss.str()));
        solved = false;
    }
    return solved;
}

void MainWindow::on_clearButton_clicked()
{
    for (int row = 0; row < m_puzzle.getRows(); ++ row)
    {
        for (int column = 0; column < m_puzzle.getColumns(); ++column)
        {
            ui->tableWidget->item(row, column)->setText("");
        }
    }
}

void MainWindow::on_solveButton_clicked()
{
    auto parsed_board = parse_board();
    if(solve(parsed_board))
    {
        ui->output->append("Solved!\n");
        set_board(parsed_board);
    }
    else
    {
        ui->output->append("Not Solved!\n");
    }
}

void MainWindow::on_setExampleButton_clicked()
{
    Board example_board;
    bool found_example = m_puzzle.get_example(example_board);
    if (found_example)
    {
        set_board(example_board);
    }
    else
    {
        ui->output->append("No example available for current board type.\n");
    }
}

void MainWindow::on_comboBox_activated(const QString &board_type_str)
{
    bool found_type = false;
    for (auto puzzle_pair : DROPDOWN_TO_PUZZLETYPE)
    {
        if (board_type_str.startsWith(QString::fromStdString(puzzle_pair.first)))
        {
            ui->output->append(QString::fromStdString("Changing board to " + puzzle_pair.first + "\n"));
            change_board_type(puzzle_pair.second);
            found_type = true;
            break;
        }
    }
    if (!found_type)
    {
        ui->output->append("Unknown board type\n");
    }
}
