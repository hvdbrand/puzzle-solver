#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "puzzle.hpp"
#include <QAbstractItemModel>
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

private slots:
    void on_regions_clicked(const QModelIndex &index);

    void on_regions_doubleClicked(const QModelIndex &index);

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    void save();
    void load();    
    void load_predefined(const std::string& puzzle_name, Settings::PuzzleType puzzle_type);
    void update_ui_for_new_puzzle();
    void color_board();
    Board parse_board();
    void set_board(Board board_to_set);

    void clear();
    void set_example();
    void solve();

    void region_buttons_set_visible(bool visible);

    Ui::MainWindow *ui;
    QAction* clearAction;
    QAction* exampleAction;
    QAction* saveAction;
    QAction* solveAction;

    const std::vector<std::pair<std::string, Settings::PuzzleType>> PREDEFINED_PUZZLES;
    const std::map<Settings::PuzzleColor, QColor> PUZZLECOLOR_TO_DISPLAYCOLOR;
    const std::vector<std::pair<QString, Settings::PuzzleColor>> COLORNAME_TO_PUZZLECOLOR;

    std::unique_ptr<Puzzle> m_puzzle;
    std::unique_ptr<QAbstractItemModel> m_model;

    QModelIndex m_selected_region;
};

#endif // MAINWINDOW_H
