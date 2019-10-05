#pragma once

#include "Ids.hpp"
#include <list>

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

using Cards = std::list<Card>;

inline bool areAdjacentAscending(const Value& lhs, const Value& rhs)
{
  switch (lhs)
  {
    case Value::ONE:
      return rhs == Value::TWO;
    case Value::TWO:
      return rhs == Value::THREE;
    case Value::THREE:
      return rhs == Value::FOUR;
    case Value::FOUR:
      return rhs == Value::FIVE;
    default:
      return false;
  }
}