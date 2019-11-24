#pragma once
#include "Card.hpp"
#include <iostream>
#include <map>
#include <unordered_map>

inline std::ostream& operator<<(std::ostream& str, Color color)
{
  switch (color)
  {
    case Color::RED:
      str << "red";
      break;
    case Color::BLUE:
      str << "blue";
      break;
    case Color::GREEN:
      str << "green";
      break;
    case Color::WHITE:
      str << "white";
      break;
    case Color::YELLOW:
      str << "yellow";
      break;
    case Color::RAINBOW:
      str << "rainbow";
      break;
    case Color::UNKNOWN:
      str << "unknown";
      break;
  }
  return str;
}

inline std::ostream& operator<<(std::ostream& str, Value value)
{
  switch (value)
  {
    case Value::ONE:
      str << "1";
      break;
    case Value::TWO:
      str << "2";
      break;
    case Value::THREE:
      str << "3";
      break;
    case Value::FOUR:
      str << "4";
      break;
    case Value::FIVE:
      str << "5";
      break;
    case Value::UNKNOWN:
      str << "unknown";
      break;
  }
  return str;
}

inline std::ostream& operator<<(std::ostream& str, const Card& card)
{
  return str << card.color << " " << card.value << " id " << card.id;
}

template<typename T>
std::ostream& operator<<(std::ostream& str, const std::list<T>& list)
{
  str << "[";
  for (auto item : list)
  {
    str << "{" << item << "}";
  }
  str << "]";
  return str;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& str, const std::map<K, V>& map)
{
  str << "[";
  for (auto item : map)
  {
    str << "{"
        << "(" << item.first << ") = " << item.second << "}";
  }
  str << "]";
  return str;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& str, const std::unordered_map<K, V>& map)
{
  str << "[";
  for (auto item : map)
  {
    str << "{"
        << "(" << item.first << ") = " << item.second << "}";
  }
  str << "]";
  return str;
}