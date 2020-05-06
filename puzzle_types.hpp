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
using PointSet = std::set<Point>;

using Region = std::pair<PointSet, Settings::PuzzleColor>;
using RegionSet = std::set<Region>;

using ExamplePoint = std::pair<Point, Value>;
using ExampleSet = std::set<ExamplePoint>;
