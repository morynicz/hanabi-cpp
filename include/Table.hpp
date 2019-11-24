#pragma once

#include "Card.hpp"
#include "Deck.hpp"
#include <array>
#include <functional>
#include <list>
#include <map>

constexpr unsigned char MAX_LIVES = 3;
constexpr unsigned char MAX_HINTS = 8;
constexpr unsigned char NUMBER_OF_COLORS = 5;

using Stacks = std::map<Color, Value>;

class NoMoreHintsAvailableException : public std::exception
{
};

class Table
{
  Deck deck;
  Stacks stacks;
  Cards graveyard;
  unsigned char numberOfHints;
  unsigned char numberOfLives;
  Composition winnableCondition;
  bool isOpeningNewStack(const Card&);
  bool isAddingCardToStack(const Card&);
  bool isStackOpened(const Color);
  bool areAllStacksFinished() const;
  void restoreHint();
  void putToGraveyard(const Card&);

public:
  Table(const Deck& deck)
    : deck(deck)
    , stacks{}
    , graveyard{}
    , numberOfHints(MAX_HINTS)
    , numberOfLives(MAX_LIVES)
    , winnableCondition(deck.getComposition())
  {
  }

  Cards getGraveyard() const { return graveyard; }
  std::map<Color, Value> getStacks() const { return stacks; }
  unsigned char getNumberOfLives() const { return numberOfLives; }
  unsigned char getNumberOfHints() const { return numberOfHints; }
  Card drawCard();
  inline bool isDeckEmpty() const { return deck.isEmpty(); }
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
