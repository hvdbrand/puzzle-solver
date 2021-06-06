#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sudokupuzzle.hpp"
#include "gcpuzzle.hpp"
#include <QAbstractItemModel>
#include <QGraphicsLineItem>
#include <QMainWindow>
#include <QPen>
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

    void on_deleteButton_clicked();

private:
    enum class PointerAtLineSegment
    {
        NO,
        VERTICAL_LINE,
        HORIZONTAL_LINE,
    };

    bool eventFilter(QObject *obj, QEvent *event);
    PointerAtLineSegment is_pointer_at_line_segment(QPoint position, int &line, int &segment) const;
    void toggle_horizontal_line(int line, int segment);
    void toggle_vertical_line(int line, int segment);
    void save();
    void load();    
    void load_predefined(const std::string& puzzle_name, Settings::PuzzleType puzzle_type);
    void update_ui_for_new_puzzle();
    void color_board();
    SudokuBoard parse_sudoku_board();
    void set_sudoku_board(SudokuBoard board_to_set);
    GcBoard parse_gc_board();
    void set_gc_board(GcBoard board_to_set);

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

    // Screen position of top left corner
    const int GC_TOP_WIDTH = 18;
    const int GC_TOP_HEIGHT = 56;
    // Dimensions of single cell
    const int GC_CELL_WIDTH = 47;
    const int GC_CELL_HEIGHT = 34;

    std::vector<std::vector<QGraphicsLineItem*>> m_horizontal_lines;
    std::vector<std::vector<QGraphicsLineItem*>> m_vertical_lines;
    QPen m_gc_pen;

    std::unique_ptr<SudokuPuzzle> m_sudoku_puzzle;
    std::unique_ptr<GcPuzzle> m_gc_puzzle;
    std::unique_ptr<QAbstractItemModel> m_model;

    const int ADD_NEW_REGION = -1;
    int m_current_region;
};

#endif // MAINWINDOW_H
