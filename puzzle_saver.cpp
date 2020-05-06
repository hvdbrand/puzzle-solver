#include "puzzle_saver.hpp"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>

namespace PuzzleSaver {
namespace  {
    void append_number(QDomDocument dom_document, QDomElement puzzle_element, const std::string& tag_name, int value)
    {
        QDomElement element = dom_document.createElement(QString::fromStdString(tag_name));
        puzzle_element.appendChild(element);
        element.appendChild(dom_document.createTextNode(QString::number(value)));
    }

    std::string colorToString(Settings::PuzzleColor color)
    {
        std::string color_str;
        static const std::map<Settings::PuzzleColor, std::string> color_map = { {Settings::PuzzleColor::NONE,   "NONE"},
                                                                                {Settings::PuzzleColor::A,      "A"},
                                                                                {Settings::PuzzleColor::B,      "B"},
                                                                                {Settings::PuzzleColor::C,      "C"},
                                                                                {Settings::PuzzleColor::D,      "D"},
                                                                                {Settings::PuzzleColor::A_DARK, "A_DARK"},
                                                                                {Settings::PuzzleColor::B_DARK, "B_DARK"},
                                                                              };
        if (color_map.count(color) > 0)
        {
            color_str = color_map.at(color);
        }
        return color_str;
    }

    void append_point(QDomDocument dom_document, QDomElement set_element, const Point& point)
    {
        QDomElement point_element = dom_document.createElement("point");
        set_element.appendChild(point_element);
        point_element.setAttribute("row", QString::number(point.first));
        point_element.setAttribute("column", QString::number(point.second));
    }

    void append_example_point(QDomDocument dom_document, QDomElement example_element, const ExamplePoint& example_point)
    {
        QDomElement example_point_element = dom_document.createElement("example_point");
        example_element.appendChild(example_point_element);
        example_point_element.setAttribute("row", QString::number(example_point.first.first));
        example_point_element.setAttribute("column", QString::number(example_point.first.second));
        example_point_element.setAttribute("value", QString::number(example_point.second));
    }

    void append_regions(QDomDocument dom_document, QDomElement sudoku_element, const RegionSet& regions)
    {
        QDomElement regions_element = dom_document.createElement("regions");
        sudoku_element.appendChild(regions_element);
        for (auto region : regions)
        {
            QDomElement region_element = dom_document.createElement("region");
            regions_element.appendChild(region_element);
            region_element.setAttribute("color", QString::fromStdString(colorToString(region.second)));
            for (auto point : region.first)
            {
                append_point(dom_document, region_element, point);
            }
        }
    }

    void append_empty_points(QDomDocument dom_document, QDomElement sudoku_element, const PointSet& empty_points)
    {
        QDomElement empty_points_element = dom_document.createElement("empty_points");
        sudoku_element.appendChild(empty_points_element);
        for (auto point : empty_points)
        {
            append_point(dom_document, empty_points_element, point);
        }
    }

    void append_example(QDomDocument dom_document, QDomElement sudoku_element, const Board& example_board)
    {
        QDomElement example_element = dom_document.createElement("example");
        const int rows = example_board.size();
        const int columns = example_board[0].size();
        for (Position row = 0; row < rows ; ++row)
        {
            for (Position column = 0; column < columns; ++column)
            {
                if (example_board[row][column] != 0)
                {
                    append_example_point(dom_document, example_element, {{row, column}, example_board[row][column]});
                }
            }
        }
        if (example_element.hasChildNodes())
        {
            sudoku_element.appendChild(example_element);
        }
    }

    void append_sudoku(QDomDocument dom_document, QDomElement puzzle_element, const Settings::Sudoku::BoardSettings& board_settings, const Board& example_board)
    {
        QDomElement sudoku_element = dom_document.createElement("sudoku");
        puzzle_element.appendChild(sudoku_element);
        append_number(dom_document, sudoku_element, "values", board_settings.values);
        append_regions(dom_document, sudoku_element, board_settings.regions);
        append_empty_points(dom_document, sudoku_element, board_settings.empty_points);
        append_example(dom_document, sudoku_element, example_board);
    }
}

void save_to_file(const Settings::Sudoku::BoardSettings& board_settings, const Board& example_board, QFile& file)
{
    QDomDocument dom_document;
    QDomElement puzzle = dom_document.createElement("puzzle");
    dom_document.appendChild(puzzle);
    append_number(dom_document, puzzle, "rows", board_settings.rows);
    append_number(dom_document, puzzle, "columns", board_settings.columns);
    append_sudoku(dom_document, puzzle, board_settings, example_board);

    QTextStream out(&file);
    out << dom_document.toString();
}

}
