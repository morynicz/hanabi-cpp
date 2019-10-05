#pragma once

#include "Card.hpp"
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
  bool isOpeningNewStack(const Card&);

public:
  Table(const Cards& deck)
    : deck(deck)
    , stacks{}
    , graveyard{}
    , numberOfHints(MAX_HINTS)
    , numberOfLives(MAX_LIVES)
  {
  }
  Cards getGraveyard() const { return graveyard; }
  std::map<Color, Value> getStacks() const { return stacks; }
  unsigned char getNumberOfLives() const { return numberOfLives; }
  unsigned char getNumberOfHints() const { return numberOfHints; }
  Card drawCard();
  inline bool isDeckEmpty() { return deck.empty(); }
  std::function<bool(const Card&)> getColorPredicate(const Color color) const;
  std::function<bool(const Card&)> getValuePredicate(const Value value) const;
  void useHintToken();
  void playCard(const Card&);
  void discard(const Card& card);
  bool isOver() const;
};
