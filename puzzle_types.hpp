#pragma once

#include <set>
#include <utility>
#include <vector>

namespace Settings {

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
using Board = std::vector<std::vector<Value>>;

using Point = std::pair<Position, Position>;
using PointVector = std::vector<Point>;
using PointSet = std::set<Point>;

using Region = std::pair<PointVector, Settings::PuzzleColor>;
using RegionVector = std::vector<Region>;

using ExamplePoint = std::pair<Point, Value>;
using ExampleSet = std::set<ExamplePoint>;
