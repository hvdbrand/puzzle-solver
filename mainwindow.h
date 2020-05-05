#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "puzzle.hpp"
#include <QMainWindow>
#include <map>
#include <memory>
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

private:
    void load();
    void load_predefined(const std::string& puzzle_name, Settings::PuzzleType puzzle_type);
    void update_ui_for_new_puzzle();
    void color_board();
    Board parse_board();
    void set_board(Board board_to_set);

    void clear();
    void set_example();
    void solve();

    Ui::MainWindow *ui;
    QAction* clearAction;
    QAction* exampleAction;
    QAction* solveAction;

    const std::vector<std::pair<std::string, Settings::PuzzleType>> PREDEFINED_PUZZLES;
    const std::map<Settings::PuzzleColor, QColor> PUZZLECOLOR_TO_DISPLAYCOLOR;

    std::unique_ptr<Puzzle> m_puzzle;
};

#endif // MAINWINDOW_H
