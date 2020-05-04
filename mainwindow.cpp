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
                            {"9x15 Twin Sudoku", Settings::PuzzleType::TWINSUDOKU_9X15},
                            {"Sudoku Mix 9x9 Base", Settings::PuzzleType::SUDOKU_MIX_9X9_TWICE}},
    PUZZLECOLOR_TO_DISPLAYCOLOR {{Settings::PuzzleColor::COLOR_A, QColor(255,0,0,50) },
                                 {Settings::PuzzleColor::COLOR_B, QColor(0,0,255,50) },
                                 {Settings::PuzzleColor::COLOR_C, QColor(0,255,0,50) },
                                 {Settings::PuzzleColor::COLOR_A_DARK, QColor(170,20,20,120) },
                                 {Settings::PuzzleColor::COLOR_B_DARK, QColor(20,20,170,120) },
                                }
{
    ui->setupUi(this);

    auto fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *loadAction = new QAction(tr("&Load puzzle..."), this);
    loadAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    loadAction->setStatusTip(tr("Load a puzzle format..."));
    connect(loadAction, &QAction::triggered, this, &MainWindow::load);
    fileMenu->addAction(loadAction);

    clearAction = new QAction(tr("&Clear puzzle"), this);
    clearAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    clearAction->setStatusTip(tr("Clear the puzzle"));
    connect(clearAction, &QAction::triggered, this, &MainWindow::clear);
    menuBar()->addAction(clearAction);

    exampleAction = new QAction(tr("Set &example puzzle"), this);
    exampleAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    exampleAction->setStatusTip(tr("Set example values for the puzzle"));
    connect(exampleAction, &QAction::triggered, this, &MainWindow::set_example);
    menuBar()->addAction(exampleAction);

    solveAction = new QAction(tr("&Solve puzzle"), this);
    solveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    solveAction->setStatusTip(tr("Solve the puzzle"));
    connect(solveAction, &QAction::triggered, this, &MainWindow::solve);
    menuBar()->addAction(solveAction);

    for (auto puzzle_type : DROPDOWN_TO_PUZZLETYPE)
    {
        ui->comboBox->addItem(QString::fromStdString(puzzle_type.first));
    }

    update_ui_for_new_puzzle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load()
{
    m_puzzle = Puzzle::loadFromFile();
    if (!m_puzzle)
    {
        ui->output->append("No puzzle was loaded.\n");
    }
    update_ui_for_new_puzzle();
}

void MainWindow::update_ui_for_new_puzzle()
{
    if (m_puzzle)
    {
        ui->tableWidget->setRowCount(m_puzzle->getRows());
        ui->tableWidget->setColumnCount(m_puzzle->getColumns());
        ui->tableWidget->clearContents();
        ui->tableWidget->clearFocus();
        int width = 30;
        for (int column = 0; column < m_puzzle->getColumns(); ++column)
        {
            ui->tableWidget->setColumnWidth(column, width);
        }

        QFont font("Helvetica", 12);
        ui->tableWidget->setFont(font);
        for (int row = 0; row < m_puzzle->getRows(); ++ row)
        {
            for (int column = 0; column < m_puzzle->getColumns(); ++column)
            {
                ui->tableWidget->setItem(row, column, new QTableWidgetItem);
                ui->tableWidget->item(row, column)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
            }
        }

        color_board();

        ui->tableWidget->setVisible(true);
        exampleAction->setEnabled(m_puzzle->has_example());
        clearAction->setEnabled(true);
        solveAction->setEnabled(true);
    }
    else
    {
        ui->tableWidget->setVisible(false);
        exampleAction->setEnabled(false);
        clearAction->setEnabled(false);
        solveAction->setEnabled(false);
    }
}

void MainWindow::color_board()
{
    for (Region region : m_puzzle->getRegions())
    {
        if (region.second != Settings::PuzzleColor::COLOR_NONE)
        {
            QColor displayColor = PUZZLECOLOR_TO_DISPLAYCOLOR.at(region.second);
            for (Point point : region.first)
            {
                ui->tableWidget->item(point.first, point.second)->setBackgroundColor(displayColor);
            }
        }
    }
}

Board MainWindow::parse_board()
{
    Board parsed(m_puzzle->getRows(), std::vector<int>(m_puzzle->getColumns()));
    QFont font("Helvetica", 14, QFont::Bold);
    for (int row = 0; row < m_puzzle->getRows(); ++row)
    {
        for (int column = 0; column < m_puzzle->getColumns(); ++column)
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
    for (int row = 0; row < m_puzzle->getRows(); ++row)
    {
        for (int column = 0; column < m_puzzle->getColumns(); ++column)
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

void MainWindow::solve()
{
    auto board = parse_board();
    auto t1 = std::chrono::high_resolution_clock::now();
    if (!m_puzzle->apply_board(board)) {
        ui->output->append("There is a contradiction in the parsed board!");
    }
    if (m_puzzle->solve()) {
        std::chrono::duration<double, std::milli> time_taken = std::chrono::high_resolution_clock::now() - t1;
        std::ostringstream ss;
        ss << "Solution found in " << time_taken.count() << " ms";
        ui->output->append(QString::fromStdString(ss.str()));
        ui->output->append("Solved!\n");
        set_board(m_puzzle->get_solution());
    } else {
        std::ostringstream ss;
        ss << "Solving the provided parsed is not possible!";
        ui->output->append(QString::fromStdString(ss.str()));
        ui->output->append("Not Solved!\n");
    }
}

void MainWindow::clear()
{
    for (int row = 0; row < m_puzzle->getRows(); ++ row)
    {
        for (int column = 0; column < m_puzzle->getColumns(); ++column)
        {
            ui->tableWidget->item(row, column)->setText("");
        }
    }
}

void MainWindow::set_example()
{
    set_board(m_puzzle->get_example());
}

void MainWindow::on_comboBox_activated(const QString &board_type_str)
{
    bool found_type = false;
    for (auto puzzle_pair : DROPDOWN_TO_PUZZLETYPE)
    {
        if (board_type_str.startsWith(QString::fromStdString(puzzle_pair.first)))
        {
            ui->output->append(QString::fromStdString("Changing board to " + puzzle_pair.first + "\n"));
            m_puzzle = std::unique_ptr<Puzzle>(new Puzzle(puzzle_pair.second));
            update_ui_for_new_puzzle();
            found_type = true;
            break;
        }
    }
    if (!found_type)
    {
        ui->output->append("Unknown board type\n");
    }
}
