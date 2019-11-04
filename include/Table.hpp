#pragma once

#include "Card.hpp"
#include <array>
#include <functional>
#include <list>
#include <map>

constexpr unsigned char MAX_LIVES = 3;
constexpr unsigned char MAX_HINTS = 8;

class NoMoreHintsAvailableException : public std::exception
{
};

class Table
{
  Cards deck;
  std::map<Color, Value> stacks;
  Cards graveyard;
  unsigned char numberOfHints;
  unsigned char numberOfLives;
  std::map<Color, std::map<Value, int>> winnableCondition;
  bool isOpeningNewStack(const Card&);
  bool isAddingCardToStack(const Card&);
  bool isStackOpened(const Color);
  void restoreHint();
  void putToGraveyard(const Card&);

public:
  Table(const Cards& deck)
    : deck(deck)
    , stacks{}
    , graveyard{}
    , numberOfHints(MAX_HINTS)
    , numberOfLives(MAX_LIVES)
    , winnableCondition{ { Color::YELLOW,
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
                         { Color::BLUE,
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
                             { Value::FIVE, 1 } } } }
  {
  }

  Cards getGraveyard() const { return graveyard; }
  std::map<Color, Value> getStacks() const { return stacks; }
  unsigned char getNumberOfLives() const { return numberOfLives; }
  unsigned char getNumberOfHints() const { return numberOfHints; }
  Card drawCard();
  inline bool isDeckEmpty() const { return deck.empty(); }
  virtual std::function<bool(const Card&)> getColorPredicate(
    const Color color) const;
  virtual std::function<bool(const Card&)> getValuePredicate(
    const Value value) const;
  void useHintToken();
  void playCard(const Card&);
  void discard(const Card& card);
  bool isOver() const;
  bool isWinnable() const;
  int getScore() const;
};
