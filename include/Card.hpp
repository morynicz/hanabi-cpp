#pragma once

#include "Ids.hpp"

enum class Color
{
  UNKNOWN,
  WHITE,
  GREEN,
  BLUE,
  RED,
  YELLOW,
  RAINBOW
};

enum class Value
{
  UNKNOWN,
  ONE,
  TWO,
  THREE,
  FOUR,
  FIVE
};

struct Card
{
  CardId id;
  Color color;
  Value value;
};