#pragma once
#include "Card.hpp"
#include <list>
#include <unordered_map>

using Composition = std::unordered_map<Color, std::unordered_map<Value, int>>;

const std::list<Card> FULL_BASIC_DECK = {
  { 0, Color::BLUE, Value::ONE },      { 1, Color::BLUE, Value::ONE },
  { 2, Color::BLUE, Value::ONE },      { 3, Color::BLUE, Value::TWO },
  { 4, Color::BLUE, Value::TWO },      { 5, Color::BLUE, Value::THREE },
  { 6, Color::BLUE, Value::THREE },    { 7, Color::BLUE, Value::FOUR },
  { 8, Color::BLUE, Value::FOUR },     { 9, Color::BLUE, Value::FIVE },
  { 10, Color::RED, Value::ONE },      { 11, Color::RED, Value::ONE },
  { 12, Color::RED, Value::ONE },      { 13, Color::RED, Value::TWO },
  { 14, Color::RED, Value::TWO },      { 15, Color::RED, Value::THREE },
  { 16, Color::RED, Value::THREE },    { 17, Color::RED, Value::FOUR },
  { 18, Color::RED, Value::FOUR },     { 19, Color::RED, Value::FIVE },
  { 20, Color::GREEN, Value::ONE },    { 21, Color::GREEN, Value::ONE },
  { 22, Color::GREEN, Value::ONE },    { 23, Color::GREEN, Value::TWO },
  { 24, Color::GREEN, Value::TWO },    { 25, Color::GREEN, Value::THREE },
  { 26, Color::GREEN, Value::THREE },  { 27, Color::GREEN, Value::FOUR },
  { 28, Color::GREEN, Value::FOUR },   { 29, Color::GREEN, Value::FIVE },
  { 30, Color::WHITE, Value::ONE },    { 31, Color::WHITE, Value::ONE },
  { 32, Color::WHITE, Value::ONE },    { 33, Color::WHITE, Value::TWO },
  { 34, Color::WHITE, Value::TWO },    { 35, Color::WHITE, Value::THREE },
  { 36, Color::WHITE, Value::THREE },  { 37, Color::WHITE, Value::FOUR },
  { 38, Color::WHITE, Value::FOUR },   { 39, Color::WHITE, Value::FIVE },
  { 40, Color::YELLOW, Value::ONE },   { 41, Color::YELLOW, Value::ONE },
  { 42, Color::YELLOW, Value::ONE },   { 43, Color::YELLOW, Value::TWO },
  { 44, Color::YELLOW, Value::TWO },   { 45, Color::YELLOW, Value::THREE },
  { 46, Color::YELLOW, Value::THREE }, { 47, Color::YELLOW, Value::FOUR },
  { 48, Color::YELLOW, Value::FOUR },  { 49, Color::YELLOW, Value::FIVE }
};

const Composition BASIC_DECK_COMPOSITION = { { Color::BLUE,
                                               { { Value::ONE, 3 },
                                                 { Value::TWO, 2 },
                                                 { Value::THREE, 2 },
                                                 { Value::FOUR, 2 },
                                                 { Value::FIVE, 1 } } },
                                             { Color::RED,
                                               { { Value::ONE, 3 },
                                                 { Value::TWO, 2 },
                                                 { Value::THREE, 2 },
                                                 { Value::FOUR, 2 },
                                                 { Value::FIVE, 1 } } },
                                             { Color::GREEN,
                                               { { Value::ONE, 3 },
                                                 { Value::TWO, 2 },
                                                 { Value::THREE, 2 },
                                                 { Value::FOUR, 2 },
                                                 { Value::FIVE, 1 } } },
                                             { Color::WHITE,
                                               { { Value::ONE, 3 },
                                                 { Value::TWO, 2 },
                                                 { Value::THREE, 2 },
                                                 { Value::FOUR, 2 },
                                                 { Value::FIVE, 1 } } },
                                             { Color::YELLOW,
                                               { { Value::ONE, 3 },
                                                 { Value::TWO, 2 },
                                                 { Value::THREE, 2 },
                                                 { Value::FOUR, 2 },
                                                 { Value::FIVE, 1 } } } };

class Deck
{
  std::list<Card> cards;
  Composition composition;

public:
  Deck(const std::list<Card>&);
  Deck(const Composition&);
  bool isEmpty() const { return cards.empty(); }
  Card draw();
  Composition getComposition() const;
};

class BasicDeck : public Deck
{
public:
  BasicDeck()
    : Deck(FULL_BASIC_DECK)
  {
  }
};