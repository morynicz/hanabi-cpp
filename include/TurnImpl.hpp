#include "Turn.hpp"
#include <functional>

class TurnImpl : public Turn
{
  std::function<void(PlayerId, Color)> giveColorHintImpl;
  std::function<void(PlayerId, Value)> giveValueHintImpl;
  std::function<void(CardId)> playCardImpl;
  std::function<void(CardId)> discardCardImpl;

public:
  TurnImpl(const std::list<CardId>& playerHand,
           const std::map<PlayerId, Cards>& otherPlayers,
           const Cards& graveyard,
           const std::map<Color, Value>& stacks,
           const unsigned char numberOfHints,
           const unsigned char numberOfLives,
           std::function<void(PlayerId, Color)> giveColorHintImpl,
           std::function<void(PlayerId, Value)> giveValueHintImpl,
           std::function<void(CardId)> playCardImpl,
           std::function<void(CardId)> discardCardImpl)
    : Turn{ playerHand, otherPlayers,  graveyard,
            stacks,     numberOfHints, numberOfLives }
    , giveColorHintImpl(giveColorHintImpl)
    , giveValueHintImpl(giveValueHintImpl)
    , playCardImpl(playCardImpl)
    , discardCardImpl(discardCardImpl)
  {
  }

  void giveHint(PlayerId playerId, Color color) override
  {
    giveColorHintImpl(playerId, color);
  };

  void giveHint(PlayerId playerId, Value value) override
  {
    giveValueHintImpl(playerId, value);
  };
  void playCard(CardId cardId) override { playCardImpl(cardId); };
  void discard(CardId cardId) override { discardCardImpl(cardId); };
  virtual ~TurnImpl() = default;
};