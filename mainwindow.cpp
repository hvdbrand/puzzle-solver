#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "puzzle_loader.hpp"
#include "puzzle_saver.hpp"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>

#include <chrono>
#include <iostream>
#include <sstream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    PREDEFINED_PUZZLES {{"9x9 Sudoku", Settings::PuzzleType::SUDOKU_9X9},
                        {"16x16 Sudoku", Settings::PuzzleType::SUDOKU_16X16},
                        {"9x15 Twin Sudoku", Settings::PuzzleType::TWINSUDOKU_9X15},
                        {"Sudoku Mix 9x9 Base", Settings::PuzzleType::SUDOKU_MIX_9X9_TWICE},
                        {"GC414WQ Hokjesgeest", Settings::PuzzleType::GC414WQ}},
    PUZZLECOLOR_TO_DISPLAYCOLOR {{Settings::PuzzleColor::A, QColor(255,0,0,50) },
                                 {Settings::PuzzleColor::B, QColor(0,0,255,50) },
                                 {Settings::PuzzleColor::C, QColor(0,255,0,50) },
                                 {Settings::PuzzleColor::D, QColor(255,255,0,50) },
                                 {Settings::PuzzleColor::A_DARK, QColor(170,20,20,120) },
                                 {Settings::PuzzleColor::B_DARK, QColor(20,20,170,120) },
                                },
    COLORNAME_TO_PUZZLECOLOR {{"None", Settings::PuzzleColor::NONE},
                              {"Red", Settings::PuzzleColor::A},
                              {"Blue", Settings::PuzzleColor::B},
                              {"Green", Settings::PuzzleColor::C},
                              {"Yellow", Settings::PuzzleColor::D},
                              {"Dark Red", Settings::PuzzleColor::A_DARK},
                              {"Dark Blue", Settings::PuzzleColor::B_DARK},
                             }
{
    ui->setupUi(this);

    auto fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *loadAction = new QAction(tr("&Load puzzle..."), this);
    loadAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    loadAction->setStatusTip(tr("Load a puzzle format..."));
    connect(loadAction, &QAction::triggered, this, &MainWindow::load);
    fileMenu->addAction(loadAction);

    saveAction = new QAction(tr("&Save current puzzle..."), this);
    saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    saveAction->setStatusTip(tr("Save a puzzle format and example..."));
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAction);

    auto predefinedMenu = fileMenu->addMenu(tr("Predefined boards"));
    int index = 1;
    for (auto predefined : PREDEFINED_PUZZLES)
    {
        std::string menuString = "&" + std::to_string(index) +  ". " + predefined.first;
        QAction* predefinedAction = new QAction(QString::fromStdString(menuString), this);
        predefinedAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1 + (index - 1)));
        connect(predefinedAction, &QAction::triggered, [=] {this->load_predefined(predefined.first, predefined.second);});
        predefinedMenu->addAction(predefinedAction);
        ++index;
    }

    QAction* quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(quitAction, &QAction::triggered, [] {QApplication::quit();});
    fileMenu->addAction(quitAction);

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

    solveAction = new QAction(tr("S&olve puzzle"), this);
    solveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    solveAction->setStatusTip(tr("Solve the puzzle"));
    connect(solveAction, &QAction::triggered, this, &MainWindow::solve);
    menuBar()->addAction(solveAction);

    for (auto color : COLORNAME_TO_PUZZLECOLOR)
    {
        ui->colorCombo->addItem(color.first, static_cast<int>(color.second));
    }

    QImage puzzleImage("../GC414WQ.jpg");
    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scene()->clear();
    scene->addPixmap(QPixmap::fromImage(puzzleImage));
    ui->graphicsView->viewport()->update();
    ui->graphicsView->scale(0.8, 0.8);

    update_ui_for_new_puzzle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::save()
{
    QFileDialog file_dialog(this, tr("Save Puzzle File"));
    file_dialog.setNameFilter("Puzzle files (*.xpuz *.xml)");
    file_dialog.setAcceptMode(QFileDialog::AcceptSave);
    file_dialog.setDirectory(QDir::homePath() + "/.puzzlesolver");
    file_dialog.setDefaultSuffix(".xpuz");
    file_dialog.setModal(true);
    QString file_name;
    if (file_dialog.exec() == QDialog::Accepted)
    {
        file_name = file_dialog.selectedFiles().first();
    }

    if (file_name.isEmpty())
        return;
    if (!file_name.endsWith(".xpuz"))
    {
        file_name += ".xpuz";
    }

    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Puzzle file"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(file_name),
                                  file.errorString()));
        return;
    }

    SudokuBoard board = parse_board();
    auto board_settings = m_sudoku_puzzle->get_board_settings();
    PuzzleSaver::save_to_file(board_settings, board, file);

    ui->output->append(tr("Puzzle was saved to file %1.\n").arg(QDir::toNativeSeparators(file_name)));
}

void MainWindow::load()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Load Puzzle File"),
                        QDir::homePath() + "/.puzzlesolver",
                       tr("Puzzle files (*.xpuz *.xml)"));
    if (file_name.isEmpty())
        return;

    QFile file(file_name);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Puzzle file"),
                             tr("Cannot open file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(file_name),
                                  file.errorString()));
        return;
    }

    m_sudoku_puzzle = PuzzleLoader::load_from_file(file);
    if (m_sudoku_puzzle)
    {
        ui->output->append(tr("Puzzle was loaded from file %1.\n").arg(QDir::toNativeSeparators(file_name)));
    }
    else
    {
        ui->output->append("No puzzle was loaded.\n");
    }
    update_ui_for_new_puzzle();
}


void MainWindow::load_predefined(const std::string& puzzle_name, Settings::PuzzleType puzzle_type)
{
    m_sudoku_puzzle.reset();
    m_gc_puzzle.reset();
    if (puzzle_type == Settings::PuzzleType::GC414WQ)
    {
        m_gc_puzzle = std::make_unique<int>(56);
        ui->output->append(QString::fromStdString("Loading predefined puzzle: " + puzzle_name + "\n"));
    }
    else // Sudoku puzzle types
    {
        m_sudoku_puzzle = std::make_unique<SudokuPuzzle>(puzzle_type);
        if (m_sudoku_puzzle)
        {
            ui->output->append(QString::fromStdString("Loading predefined puzzle: " + puzzle_name + "\n"));
        }
        else
        {
            ui->output->append("Failed to load the predefined puzzle.\n");
        }
    }
    update_ui_for_new_puzzle();
}

void MainWindow::update_ui_for_new_puzzle()
{
    m_model.reset();
    ui->regions->setVisible(false);
    ui->tableWidget->setVisible(false);
    exampleAction->setEnabled(false);
    clearAction->setEnabled(false);
    saveAction->setEnabled(false);
    solveAction->setEnabled(false);
    region_buttons_set_visible(false);
    ui->graphicsView->setVisible(false);
    if (m_sudoku_puzzle)
    {
        ui->tableWidget->setRowCount(m_sudoku_puzzle->get_rows());
        ui->tableWidget->setColumnCount(m_sudoku_puzzle->get_columns());
        ui->tableWidget->clearContents();
        ui->tableWidget->clearFocus();
        int width = 30;
        for (int column = 0; column < m_sudoku_puzzle->get_columns(); ++column)
        {
            ui->tableWidget->setColumnWidth(column, width);
        }

        QFont font("Helvetica", 12);
        ui->tableWidget->setFont(font);
        for (Position row = 0; row < m_sudoku_puzzle->get_rows(); ++row)
        {
            for (Position column = 0; column < m_sudoku_puzzle->get_columns(); ++column)
            {
                ui->tableWidget->setItem(row, column, new QTableWidgetItem);
                ui->tableWidget->item(row, column)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
            }
        }

        color_board();

        m_model = std::unique_ptr<QAbstractItemModel>(new QStandardItemModel);
        ui->regions->setModel(m_model.get());
        m_model->insertColumn(0);
        ui->regions->setColumnWidth(0, 120);

        m_model->insertRows(0, m_sudoku_puzzle->get_regions().size() + 1);
        int i = 0;
        for(auto region : m_sudoku_puzzle->get_regions())
        {
            std::ostringstream ss;
            ss << "(" << region.first.front().first << "," << region.first.front().second << ") -- ("
               << region.first.back().first << "," << region.first.back().second << ")";
            QMap<int, QVariant> map;
            map[Qt::DisplayRole] = QVariant(ss.str().c_str());
            map[Qt::UserRole] = QVariant(i);
            if (region.second != Settings::PuzzleColor::NONE)
            {
                map[Qt::BackgroundRole] = QBrush(PUZZLECOLOR_TO_DISPLAYCOLOR.at(region.second));
            }
            m_model->setItemData(m_model->index(i, 0), map);
            i++;
        }
        QMap<int, QVariant> map;
        map[Qt::DisplayRole] = QVariant("Add new region...");
        map[Qt::UserRole] = QVariant(ADD_NEW_REGION);
        m_model->setItemData(m_model->index(i, 0), map);

        ui->regions->setVisible(true);
        ui->tableWidget->setVisible(true);
        exampleAction->setEnabled(m_sudoku_puzzle->has_example());
        clearAction->setEnabled(true);
        saveAction->setEnabled(true);
        solveAction->setEnabled(true);
        region_buttons_set_visible(false);
    }
    else if (m_gc_puzzle)
    {
        ui->graphicsView->setVisible(true);
    }
}

void MainWindow::color_board()
{
    for (Region region : m_sudoku_puzzle->get_regions())
    {
        if (region.second != Settings::PuzzleColor::NONE)
        {
            QColor displayColor = PUZZLECOLOR_TO_DISPLAYCOLOR.at(region.second);
            for (Point point : region.first)
            {
                ui->tableWidget->item(point.first, point.second)->setBackgroundColor(displayColor);
            }
        }
    }
}

SudokuBoard MainWindow::parse_board()
{
    const Position rows = m_sudoku_puzzle->get_rows();
    const Position columns = m_sudoku_puzzle->get_columns();
    SudokuBoard parsed(rows, std::vector<int>(columns));
    QFont font("Helvetica", 14, QFont::Bold);
    for (Position row = 0; row < rows; ++row)
    {
        for (Position column = 0; column < columns; ++column)
        {
            QTableWidgetItem* item = ui->tableWidget->item(row, column);
            if (item != nullptr)
            {
                parsed[row][column] = item->text().toInt();
                if (parsed[row][column] != 0)
                {
                    ui->tableWidget->item(row, column)->setFont(font);
                }
            }
        }
    }
    return parsed;
}

void MainWindow::set_sudoku_board(SudokuBoard board_to_set)
{
    QFont font("Helvetica", 12);
    for (Position row = 0; row < m_sudoku_puzzle->get_rows(); ++row)
    {
        for (Position column = 0; column < m_sudoku_puzzle->get_columns(); ++column)
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
    if (m_sudoku_puzzle)
    {
        auto board = parse_board();
        auto t1 = std::chrono::high_resolution_clock::now();
        if (!m_sudoku_puzzle->apply_board(board)) {
            ui->output->append("There is a contradiction in the parsed board!");
        }
        if (m_sudoku_puzzle->solve()) {
            std::chrono::duration<double, std::milli> time_taken = std::chrono::high_resolution_clock::now() - t1;
            std::ostringstream ss;
            ss << "Solution found in " << time_taken.count() << " ms";
            ui->output->append(QString::fromStdString(ss.str()));
            set_sudoku_board(m_sudoku_puzzle->get_solution());
        } else {
            std::ostringstream ss;
            ss << "Solving the provided parsed is not possible!";
            ui->output->append(QString::fromStdString(ss.str()));
        }
    }
}

void MainWindow::clear()
{
    QFont font("Helvetica", 12);
    if (m_sudoku_puzzle)
    {
        for (Position row = 0; row < m_sudoku_puzzle->get_rows(); ++row)
        {
            for (Position column = 0; column < m_sudoku_puzzle->get_columns(); ++column)
            {
                ui->tableWidget->item(row, column)->setText("");
                ui->tableWidget->item(row, column)->setFont(font);
            }
        }
    }
}

void MainWindow::set_example()
{
    if (m_sudoku_puzzle)
    {
        set_sudoku_board(m_sudoku_puzzle->get_example());
    }
}

void MainWindow::on_regions_clicked(const QModelIndex &index)
{
    int region_index = index.data(Qt::UserRole).toInt();
    if (region_index == ADD_NEW_REGION)
    {
        ui->colorCombo->setCurrentText(COLORNAME_TO_PUZZLECOLOR.at(0).first);
        return;
    }
    ui->tableWidget->clearSelection();
    auto region = m_sudoku_puzzle->get_regions()[region_index];
    for (auto point : region.first)
    {
        auto model_index = ui->tableWidget->model()->index(point.first, point.second);
        ui->tableWidget->selectionModel()->select(model_index, QItemSelectionModel::Select);
    }
    for (auto color : COLORNAME_TO_PUZZLECOLOR)
    {
        if (color.second == region.second)
        {
            ui->colorCombo->setCurrentText(color.first);
        }
    }
}

void MainWindow::on_regions_doubleClicked(const QModelIndex &index)
{
    region_buttons_set_visible(true);
    m_current_region = index.data(Qt::UserRole).toInt();
}

void MainWindow::region_buttons_set_visible(bool visible)
{
    ui->okButton->setVisible(visible);
    ui->cancelButton->setVisible(visible);
    ui->deleteButton->setVisible(visible);
    ui->colorCombo->setVisible(visible);
}

void MainWindow::on_okButton_clicked()
{
    Region region;
    region.second = static_cast<Settings::PuzzleColor>(ui->colorCombo->currentData().toInt());
    for (auto selection : ui->tableWidget->selectionModel()->selection())
    {
        for (auto index : selection.indexes())
        {
            region.first.emplace_back(index.row(), index.column());
        }
    }
    if (region.first.size() != m_sudoku_puzzle->get_values())
    {
        ui->output->append(".\n");
        QMessageBox::warning(this, tr("Invalid region"),
                             tr("Region contains %1 points, but should contain %2 points.")
                             .arg(region.first.size()).arg(m_sudoku_puzzle->get_values()));
    }
    else
    {
        if (m_current_region == ADD_NEW_REGION)
        {
            m_sudoku_puzzle->add_region(region);
        }
        else
        {
            m_sudoku_puzzle->replace_region(m_current_region, region);
        }
        update_ui_for_new_puzzle();
    }
}

void MainWindow::on_cancelButton_clicked()
{
    region_buttons_set_visible(false);
}

void MainWindow::on_deleteButton_clicked()
{
    m_sudoku_puzzle->remove_region(m_current_region);
    update_ui_for_new_puzzle();
}
