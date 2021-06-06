#pragma once

#include <set>
#include <utility>
#include <vector>

namespace Settings {

enum class PuzzleType
{
  SUDOKU_9X9,
  SUDOKU_16X16,
  TWINSUDOKU_9X15,
  SUDOKU_MIX_9X9_TWICE,
  GC414WQ,
};

enum class PuzzleColor
{
  NONE,
  A,
  B,
  C,
  D,
  A_DARK,
  B_DARK,
};

}

using Value = int;
using Position = int;
using SudokuBoard = std::vector<std::vector<Value>>;

using Point = std::pair<Position, Position>;
using PointVector = std::vector<Point>;
using PointSet = std::set<Point>;

using Region = std::pair<PointVector, Settings::PuzzleColor>;
using RegionVector = std::vector<Region>;

using ExamplePoint = std::pair<Point, Value>;
using ExampleSet = std::set<ExamplePoint>;

using GcSegments = std::vector<std::vector<bool>>;
struct GcBoard
{
    GcSegments horizontal_lines;
    GcSegments vertical_lines;
};
