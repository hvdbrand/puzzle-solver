#pragma once

#include <set>
#include <utility>
#include <vector>

namespace Settings {

enum class PuzzleColor
{
  COLOR_NONE,
  COLOR_A,
  COLOR_B,
  COLOR_C,
  COLOR_A_DARK,
  COLOR_B_DARK,
};

}

using Board = std::vector<std::vector<int>>;
using Point = std::pair<int, int>;
using PointSet = std::set<Point>;
using Region = std::pair<PointSet, Settings::PuzzleColor>;
using RegionSet = std::set<Region>;
