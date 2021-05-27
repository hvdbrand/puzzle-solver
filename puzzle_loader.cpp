#include "puzzle_loader.hpp"

#include <QDomDocument>
#include <QFile>

#include <iostream>

namespace PuzzleLoader {

namespace {
    Settings::PuzzleColor stringToColor(const QString& color_str)
    {
        Settings::PuzzleColor color = Settings::PuzzleColor::NONE;
        static const std::map<std::string, Settings::PuzzleColor> color_map = { {"NONE", Settings::PuzzleColor::NONE},
                                                                                {"A", Settings::PuzzleColor::A},
                                                                                {"B", Settings::PuzzleColor::B},
                                                                                {"C", Settings::PuzzleColor::C},
                                                                                {"D", Settings::PuzzleColor::D},
                                                                                {"A_DARK", Settings::PuzzleColor::A_DARK},
                                                                                {"B_DARK", Settings::PuzzleColor::B_DARK},
                                                                              };
        if (color_map.count(color_str.toStdString()) > 0)
        {
            color = color_map.at(color_str.toStdString());
        }
        return color;
    }

    bool parse_point_from_element(QDomElement pointElement, Point& point, const Settings::Sudoku::BoardSettings& board_settings)
    {
        bool success = false;
        if ((pointElement.tagName() == "point") && (pointElement.attributes().contains("row")) && (pointElement.attributes().contains("column")))
        {
            bool valid_row, valid_column;
            int row = pointElement.attributeNode("row").value().toInt(&valid_row);
            int column = pointElement.attributeNode("column").value().toInt(&valid_column);
            if (valid_row && valid_column &&
                    (row >= 0) && (row < board_settings.rows) &&
                    (column >= 0) && (column < board_settings.columns))
            {
                point.first = row;
                point.second = column;
                success = true;
            }
        }
        return success;
    }

    bool parse_regions_from_element(QDomElement regionsElement, Settings::Sudoku::BoardSettings& board_settings)
    {
        bool success = true;
        for (QDomElement element = regionsElement.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
        {
            if ((element.tagName() != "region") || (!element.attributes().contains("color")))
            {
                success = false;
                break;
            }
            Region region;
            region.second = stringToColor(element.attributeNode("color").value());
            for (QDomElement pointElement = element.firstChildElement(); !pointElement.isNull(); pointElement = pointElement.nextSiblingElement())
            {
                Point point;
                if(parse_point_from_element(pointElement, point, board_settings))
                {
                    region.first.push_back(point);
                }
            }
            if (region.first.size() != board_settings.values)
            {
                success = false;
                break;
            }
            board_settings.regions.push_back(region);
        }
        return success;
    }

    void parse_empty_points_from_element(QDomElement empty_points_element, Settings::Sudoku::BoardSettings& board_settings)
    {
        for (QDomElement point_element = empty_points_element.firstChildElement(); !point_element.isNull(); point_element = point_element.nextSiblingElement())
        {
            Point point;
            if(parse_point_from_element(point_element, point, board_settings))
            {
                board_settings.empty_points.insert(point);
            }
        }
    }

    void parse_example_from_element(QDomElement example_element, Settings::Sudoku::BoardSettings& board_settings)
    {
        for (QDomElement example_point = example_element.firstChildElement(); !example_point.isNull(); example_point = example_point.nextSiblingElement())
        {
            if ((example_point.tagName() == "example_point") &&
                (example_point.attributes().contains("row")) &&
                (example_point.attributes().contains("column")) &&
                (example_point.attributes().contains("value")))
            {
                bool valid_row, valid_column, valid_value;
                int row = example_point.attributeNode("row").value().toInt(&valid_row);
                int column = example_point.attributeNode("column").value().toInt(&valid_column);
                int value = example_point.attributeNode("value").value().toInt(&valid_value);
                if (valid_row && valid_column && valid_value &&
                        (row >= 0) && (row < board_settings.rows) &&
                        (column >= 0) && (column < board_settings.columns) &&
                        (value > board_settings.min_value) && (value <= board_settings.max_value))
                {
                    board_settings.example.insert({{row, column}, value});
                }
            }
        }
    }

    bool parse_sudoku_settings_from_element(QDomElement sudoku_element, Settings::Sudoku::BoardSettings& board_settings)
    {
        bool found_values = false;
        bool found_regions = false;

        board_settings.empty_points.clear();
        board_settings.has_example = false;

        for (QDomElement element = sudoku_element.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
        {
            if (element.tagName() == "values")
            {
                QDomText text = element.firstChild().toText();
                if (!text.isNull())
                {
                    bool validInt;
                    int values = text.data().toInt(&validInt);
                    if (validInt && (values == 9))
                    {
                        board_settings.values = 9;
                        board_settings.min_value = 0;
                        board_settings.max_value = 9;
                        found_values = true;
                    }
                    else if (validInt && (values == 16))
                    {
                        board_settings.values = 16;
                        board_settings.min_value = 0;
                        board_settings.max_value = 16;
                        found_values = true;
                    }
                }
            }
            else if (element.tagName() == "regions")
            {
                found_regions = parse_regions_from_element(element, board_settings);
            }
            else if (element.tagName() == "empty_points")
            {
                parse_empty_points_from_element(element, board_settings);
            }
            else if (element.tagName() == "example")
            {
                board_settings.has_example = true;
                parse_example_from_element(element, board_settings);
            }
        }
        return found_values && found_regions;
    }

    bool load_settings_from_file(QIODevice *device, Settings::Sudoku::BoardSettings& board_settings)
    {
        bool success = false;
        QDomDocument dom_document;

        QString errorStr;
        int errorLine;
        int errorColumn;

        if (dom_document.setContent(device, true, &errorStr, &errorLine, &errorColumn))
        {
            // TODO: Verify xml schema
            QDomElement root = dom_document.documentElement();
            if (root.tagName() == "puzzle")
            {
                bool found_rows = false;
                bool found_columns = false;
                bool found_sudoku = false;
                for (QDomElement element = root.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
                {
                    if (element.tagName() == "rows")
                    {
                        QDomText text = element.firstChild().toText();
                        if (!text.isNull())
                        {
                            board_settings.rows = text.data().toInt(&found_rows);
                        }
                    }
                    else if (element.tagName() == "columns")
                    {
                        QDomText text = element.firstChild().toText();
                        if (!text.isNull())
                        {
                            board_settings.columns = text.data().toInt(&found_columns);
                        }
                    }
                    else if (element.tagName() == "sudoku")
                    {
                        found_sudoku = parse_sudoku_settings_from_element(element, board_settings) ;
                    }
                }
                success = found_rows && found_columns && found_sudoku;
            }
        }
        else
        {
            std::cout << "Parse error at line " << errorLine << ", column "
                      << errorColumn << ":\n" << errorStr.toStdString() << std::endl;
        }
        return success;
    }
}

std::unique_ptr<SudokuPuzzle> load_from_file(QFile& file)
{
    std::unique_ptr<SudokuPuzzle> puzzle;
    Settings::Sudoku::BoardSettings board_settings;
    if (load_settings_from_file(&file, board_settings))
    {
        puzzle = std::unique_ptr<SudokuPuzzle>(new SudokuPuzzle(board_settings));
    }
    return puzzle;
}

}

