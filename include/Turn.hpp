#pragma once

#include "Card.hpp"
#include "Ids.hpp"
#include <functional>
#include <list>
#include <map>

class Turn
{
  std::function<void(PlayerId, Color)> giveColorHintImpl;
  std::function<void(PlayerId, Value)> giveValueHintImpl;
  std::function<void(CardId)> playCardImpl;
  std::function<void(CardId)> discardCardImpl;

public:
  Turn(const std::list<CardId>& playerHand,
       const std::map<PlayerId, Cards>& otherPlayers,
       const Cards& graveyard,
       const std::map<Color, Value>& stacks,
       const unsigned char numberOfHints,
       const unsigned char numberOfLives,
       std::function<void(PlayerId, Color)> giveColorHintImpl,
       std::function<void(PlayerId, Value)> giveValueHintImpl,
       std::function<void(CardId)> playCardImpl,
       std::function<void(CardId)> discardCardImpl)
    : giveColorHintImpl(giveColorHintImpl)
    , giveValueHintImpl(giveValueHintImpl)
    , playCardImpl(playCardImpl)
    , discardCardImpl(discardCardImpl)
    , playerHand(playerHand)
    , otherPlayers(otherPlayers)
    , graveyard(graveyard)
    , stacks(stacks)
    , numberOfHints(numberOfHints)
    , numberOfLives(numberOfLives)
  {
  }

  const std::list<CardId> playerHand;
  const std::map<PlayerId, Cards> otherPlayers;
  const Cards graveyard;
  const std::map<Color, Value> stacks;
  const unsigned char numberOfHints;
  const unsigned char numberOfLives;

  void giveHint(PlayerId playerId, Color color)
  {
    giveColorHintImpl(playerId, color);
  };

  void giveHint(PlayerId playerId, Value value)
  {
    giveValueHintImpl(playerId, value);
  };
  void playCard(CardId cardId) { playCardImpl(cardId); };
  void discard(CardId cardId) { discardCardImpl(cardId); };
  virtual ~Turn() = default;
};