#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "puzzle.hpp"
#include <QMainWindow>
#include <map>
#include <vector>

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
    void change_board_type(Settings::PuzzleType new_puzzle_type);
    void color_board();
    Board parse_board();
    void set_board(Board board_to_set);
    bool solve(Board &board_to_solve);

    Ui::MainWindow *ui;

    const std::vector<std::pair<std::string, Settings::PuzzleType>> DROPDOWN_TO_PUZZLETYPE;
    const std::map<Settings::PuzzleColor, QColor> PUZZLECOLOR_TO_DISPLAYCOLOR;

    Puzzle m_puzzle;
};

#endif // MAINWINDOW_H
