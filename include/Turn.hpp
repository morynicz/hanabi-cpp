#pragma once

#include "Card.hpp"
#include "Ids.hpp"
#include <list>
#include <map>

using Hand = std::list<Card>;

class Turn
{
public:
  Turn(std::list<CardId> playerHand,
       std::map<PlayerId, Hand> otherPlayers,
       int numberOfHints)
    : playerHand(playerHand)
    , otherPlayers(otherPlayers)
    , numberOfHints(numberOfHints)
  {
  }

  const std::list<CardId> playerHand;
  const std::map<PlayerId, Hand> otherPlayers;
  const int numberOfHints;

  virtual void giveHint(PlayerId, Color) = 0;
  virtual void giveHint(PlayerId, Value) = 0;
  virtual void playCard(CardId) = 0;
  virtual void discard(CardId) = 0;
  virtual ~Turn() = default;
};