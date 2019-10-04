#pragma once

#include "Card.hpp"
#include "Ids.hpp"
#include <list>
#include <map>

using Cards = std::list<Card>;

class Turn
{
public:
  Turn(std::list<CardId> playerHand,
       std::map<PlayerId, Cards> otherPlayers,
       Cards graveyard,
       int numberOfHints)
    : playerHand(playerHand)
    , otherPlayers(otherPlayers)
    , graveyard(graveyard)
    , numberOfHints(numberOfHints)
  {
  }

  const std::list<CardId> playerHand;
  const std::map<PlayerId, Cards> otherPlayers;
  const Cards graveyard;
  const int numberOfHints;

  virtual void giveHint(PlayerId, Color) = 0;
  virtual void giveHint(PlayerId, Value) = 0;
  virtual void playCard(CardId) = 0;
  virtual void discard(CardId) = 0;
  virtual ~Turn() = default;
};