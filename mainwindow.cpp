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
    change_board_type(Settings::Sudoku::PuzzleType::SUDOKU_9X9);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::change_board_type(Settings::Sudoku::PuzzleType new_puzzle_type)
{
    m_puzzle_type = new_puzzle_type;
    m_board_settings = Settings::Sudoku::get_board_settings(new_puzzle_type);

    ui->tableWidget->setRowCount(m_board_settings.rows);
    ui->tableWidget->setColumnCount(m_board_settings.columns);
    ui->tableWidget->clearContents();
    ui->tableWidget->clearFocus();
    int width = 30;
    for (int column = 0; column < m_board_settings.columns; ++column)
    {
        ui->tableWidget->setColumnWidth(column, width);
    }

    QFont font("Helvetica", 12);
    ui->tableWidget->setFont(font);
    for (int row = 0; row < m_board_settings.rows; ++ row)
    {
        for (int column = 0; column < m_board_settings.columns; ++column)
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
    for (int row = 0; row < m_board_settings.rows; row += m_board_settings.minibox_size)
    {
        bool color_minibox = start_row_with_colored_minibox;
        for (int column = 0; column < m_board_settings.columns; column += m_board_settings.minibox_size)
        {
            QColor color = color_minibox ? baseColor1 : baseColor2;
            for (int minibox_row = 0; minibox_row < m_board_settings.minibox_size; ++minibox_row)
            {
                for (int minibox_column = 0; minibox_column < m_board_settings.minibox_size; ++minibox_column)
                {
                    ui->tableWidget->item(row + minibox_row, column + minibox_column)->setBackgroundColor(color);
                }
            }
            color_minibox = !color_minibox;
        }
        start_row_with_colored_minibox = !start_row_with_colored_minibox;
    }
    // For the twin sudoku, recolor the middle region
    if (m_puzzle_type == Settings::Sudoku::PuzzleType::TWINSUDOKU_9X15)
    {
        QColor middleRegionColor1(170,20,20,120);
        QColor middleRegionColor2(20,20,170,120);
        for (int row = 0; row < m_board_settings.rows; ++row)
        {
            for (int column = 6; column < 9; ++column)
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
    Board parsed(m_board_settings.rows, std::vector<int>(m_board_settings.columns));
    QFont font("Helvetica", 14, QFont::Bold);
    for (int row = 0; row < m_board_settings.rows; ++row)
    {
        for (int column = 0; column < m_board_settings.columns; ++column)
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
    for (int row = 0; row < m_board_settings.rows; ++row)
    {
        for (int column = 0; column < m_board_settings.columns; ++column)
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
    for (int row = 0; row < m_board_settings.rows; ++ row)
    {
        for (int column = 0; column < m_board_settings.columns; ++column)
        {
            ui->tableWidget->item(row, column)->setText("");
        }
    }
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
    if (m_puzzle_type == Settings::Sudoku::PuzzleType::SUDOKU_9X9)
    {
        Board example_board;
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
        set_board(example_board);
    }
    else if (m_puzzle_type == Settings::Sudoku::PuzzleType::SUDOKU_16X16)
    {
        Board example_board;
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
        set_board(example_board);
    }
    else if (m_puzzle_type == Settings::Sudoku::PuzzleType::TWINSUDOKU_9X15)
    {
        Board example_board;
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
        change_board_type(Settings::Sudoku::PuzzleType::SUDOKU_9X9);
    }
    else if (board_type_str.startsWith("16 x 16"))
    {
        ui->output->append("Changing board to 16 x 16 Sudoku\n");
        change_board_type(Settings::Sudoku::PuzzleType::SUDOKU_16X16);
    }
    else if (board_type_str.startsWith("9 x 15 Twin"))
    {
        ui->output->append("Changing board to 9 x 15 Twin Sudoku\n");
        change_board_type(Settings::Sudoku::PuzzleType::TWINSUDOKU_9X15);
    }
    else
    {
        ui->output->append("Unknown board type\n");
    }
}
