#pragma once

#include "Card.hpp"
#include "Ids.hpp"
#include <list>
#include <map>

struct Turn
{
  const std::list<Card> playerHand;
  const std::map<PlayerId, std::list<Card>> otherPlayers;
  virtual void giveHint(PlayerId, Color) = 0;
  virtual void giveHint(PlayerId, Value) = 0;
  virtual void playCard(CardId) = 0;
  virtual void discard(CardId) = 0;
};