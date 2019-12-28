#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <chrono>
#include <iostream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    change_board_type(Settings::Sudoku::Size::SIZE_9X9);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::change_board_type(Settings::Sudoku::Size new_size)
{
    m_board_size = new_size;
    m_board_settings = Settings::Sudoku::get_board_settings(new_size);
    ui->tableWidget->setRowCount(m_board_settings.rows);
    ui->tableWidget->setColumnCount(m_board_settings.columns);
    ui->tableWidget->clearContents();
    ui->tableWidget->clearFocus();
    QFont font("Helvetica", 12);
    ui->tableWidget->setFont(font);
}

Board MainWindow::parse_board()
{
    Board parsed(m_board_settings.rows, std::vector<int>(m_board_settings.columns));
    QFont font("Helvetica", 14, QFont::Bold);
    for (int row = 0; row < m_board_settings.rows; ++row)
    {
        for (int column = 0; column < m_board_settings.columns; ++column)
        {
            QTableWidgetItem* item = ui->tableWidget->item(row, column);
            if (item != nullptr)
            {
//                std::cout << "(" << row << ", " << column << ") = " << item->text().toInt() << std::endl;
                parsed[row][column] = item->text().toInt();
                ui->tableWidget->item(row, column)->setFont(font);
            }
        }
    }
    return parsed;
}

void MainWindow::set_board(Board board_to_set)
{
    for (int row = 0; row < m_board_settings.rows; ++row)
    {
        for (int column = 0; column < m_board_settings.columns; ++column)
        {
            QTableWidgetItem* item = ui->tableWidget->item(row, column);
            bool not_set_yet = (item == nullptr) || (item->text().length() == 0);
            if ((board_to_set[row][column] != 0) && (not_set_yet))
            {
//                std::cout << "(" << row << ", " << column << ") = " << board_to_set[row][column] << std::endl;
                ui->tableWidget->setItem(row, column, new QTableWidgetItem(QString::number(board_to_set[row][column])));
            }
        }
    }
}

bool MainWindow::solve(Board &board_to_solve)
{
    bool solved = false;
    auto t1 = std::chrono::high_resolution_clock::now();
    Solver s(m_board_settings);
    if (!s.apply_board(board_to_solve)) {
        ui->output->append("There is a contradiction in the parsed!");
        solved = false;
    }
    if (s.solve()) {
        std::chrono::duration<double, std::milli> time_taken = std::chrono::high_resolution_clock::now() - t1;
        std::ostringstream ss;
        ss << "Solution found in " << time_taken.count() << " ms";
        ui->output->append(QString::fromStdString(ss.str()));
        board_to_solve = s.get_solution();
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
    ui->tableWidget->clearContents();
    QFont font("Helvetica", 12);
    ui->tableWidget->setFont(font);
}

void MainWindow::on_solveButton_clicked()
{
    auto parsed_board = parse_board();
    set_board(parsed_board);
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
    if (m_board_size == Settings::Sudoku::Size::SIZE_9X9)
    {
        Board example_board(m_board_settings.rows, std::vector<int>(m_board_settings.columns));
        example_board[0][1] = 2;
        example_board[0][2] = 6;
        example_board[0][6] = 8;
        example_board[0][7] = 1;
        example_board[1][0] = 3;
        example_board[1][3] = 7;
        example_board[1][5] = 8;
        example_board[1][8] = 6;
        example_board[2][0] = 4;
        example_board[2][4] = 5;
        example_board[2][8] = 7;
        example_board[3][1] = 5;
        example_board[3][3] = 1;
        example_board[3][5] = 7;
        example_board[3][7] = 9;
        example_board[4][2] = 3;
        example_board[4][3] = 9;
        example_board[4][5] = 5;
        example_board[4][6] = 1;
        example_board[5][1] = 4;
        example_board[5][3] = 3;
        example_board[5][5] = 2;
        example_board[5][7] = 5;
        example_board[6][0] = 1;
        example_board[6][4] = 3;
        example_board[6][8] = 2;
        example_board[7][0] = 5;
        example_board[7][3] = 2;
        example_board[7][5] = 4;
        example_board[7][8] = 9;
        example_board[8][1] = 3;
        example_board[8][2] = 8;
        example_board[8][6] = 4;
        example_board[8][7] = 6;
        set_board(example_board);
    }
    else if (m_board_size == Settings::Sudoku::Size::SIZE_16X16)
    {
        Board example_board;//(m_board_settings.rows, std::vector<int>(m_board_settings.columns));
        std::vector<int> row;
        //  1   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 0,  3, 11,  0,  0,  0, 10, 16,  0,  0,  0,  0,  0,  5,  6,  4};
        example_board.push_back(row);
        //  2   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 0,  0,  0,  0,  3,  0,  0,  0,  0, 11,  6,  0, 14,  0, 15,  1};
        example_board.push_back(row);
        //  3   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 0,  0,  1,  0,  7, 15,  0,  0,  3, 14,  0, 16, 12,  0,  0,  0};
        example_board.push_back(row);
        //  4   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 0, 14,  0,  0,  8, 12,  6,  0,  0,  0, 13,  0, 16,  9,  0,  0};
        example_board.push_back(row);
        //  5   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 0,  4,  0,  0,  0,  0,  8, 12,  0,  0,  0,  7,  0,  3, 14, 15};
        example_board.push_back(row);
        //  6   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = {10,  0,  0, 14, 11,  3,  0,  0,  9,  8,  0,  0,  0,  0,  7,  0};
        example_board.push_back(row);
        //  7   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 3,  2, 12,  7, 16,  9,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0};
        example_board.push_back(row);
        //  8   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = {13,  0,  0,  0, 14,  0,  7,  0,  0,  2,  0,  0,  0,  6,  0,  0};
        example_board.push_back(row);
        //  9   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 0,  0,  8,  0,  0,  0, 14,  0,  0,  7,  0,  4,  0,  0,  0, 13};
        example_board.push_back(row);
        // 10   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 0,  0,  0,  0,  0,  0, 16,  0,  0,  0, 15,  8,  4, 11,  3,  2};
        example_board.push_back(row);
        // 11   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 0, 16,  0,  0,  0,  0, 13,  8,  0,  0,  5, 14, 15,  0,  0,  7};
        example_board.push_back(row);
        // 12   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = {12,  9,  3,  0,  4,  0,  0,  0,  2, 13,  0,  0,  0,  0,  5,  0};
        example_board.push_back(row);
        // 13   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 0,  0, 13, 10,  0,  8,  0,  0,  0, 12,  7, 11,  0,  0, 16,  0};
        example_board.push_back(row);
        // 14   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 0,  0,  0,  1,  9,  0,  5, 11,  0,  0, 14,  6,  0,  7,  0,  0};
        example_board.push_back(row);
        // 15   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 5,  7,  0,  8,  0,  4, 12,  0,  0,  0,  0,  2,  0,  0,  0,  0};
        example_board.push_back(row);
        // 16   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
        row = { 4, 11, 14,  0,  0,  0,  0,  0,  8, 15,  0,  0,  0, 13, 10,  0};
        example_board.push_back(row);



        set_board(example_board);
    }
    else
    {
        ui->output->append("No example available for given board type.\n");
    }
}

void MainWindow::on_comboBox_activated(const QString &board_type_str)
{
    if (board_type_str == "9 x 9")
    {
        ui->output->append("Changing board to 9 x 9 Sudoku\n");
        change_board_type(Settings::Sudoku::Size::SIZE_9X9);
    }
    else if (board_type_str.startsWith("16 x 16"))
    {
        ui->output->append("Changing board to 16 x 16 Sudoku\n");
        change_board_type(Settings::Sudoku::Size::SIZE_16X16);
    }
    else
    {
        ui->output->append("Unknown board type\n");
    }
}
