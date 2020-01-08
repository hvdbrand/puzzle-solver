#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "solver.hpp"
#include "sudokusettings.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_clearButton_clicked();

    void on_solveButton_clicked();

    void on_setExampleButton_clicked();

    void on_comboBox_activated(const QString &m_board_type);

private:
    void change_board_type(Settings::Sudoku::PuzzleType new_size);
    void color_miniboxes();
    Board parse_board();
    void set_board(Board board_to_set);
    bool solve(Board &board_to_solve);


    Settings::Sudoku::PuzzleType m_puzzle_type;
    Settings::Sudoku::BoardSettings m_board_settings;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
