#include "Game.hpp"
#include <algorithm>
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

std::list<CardId> transformToCardIds(const std::map<PlayerId, Cards>& hands,
                                     PlayerId playerId)
{
  std::list<CardId> cardIds;
  std::transform(hands.at(playerId).begin(),
                 hands.at(playerId).end(),
                 std::back_inserter(cardIds),
                 [](const Card& card) { return card.id; });
  return cardIds;
}

Game::Game(const Players& players, std::list<Card> deck)
  : players(players)
  , table{ deck }
  , hands()
  , currentPlayer(players.cbegin())
{
  validate();
  play();
}

void Game::validate()
{
  if (players.size() < 2)
    throw NotEnoughPlayersException();
  if (players.size() > 5)
    throw TooManyPlayersException();
}

void Game::play()
{
  currentPlayer = players.cbegin(); // this is some weird stuff
  for (auto player : players)
  {
    hands.insert({ player->getId(), {} });
  }
  dealCards();
}

void Game::turn()
{
  if (table.isOver())
    throw GameIsOverException();
  runPlayerTurn(**currentPlayer);
  advancePlayer(currentPlayer);
}

void Game::advancePlayer(Players::const_iterator& playersIt)
{
  playersIt++;
  if (playersIt == players.cend())
    playersIt = players.cbegin();
}

void Game::dealCards()
{
  auto playersIt = players.cbegin();
  for (size_t i = 0; i < 10; ++i)
  {
    drawCard((*playersIt)->getId());
    advancePlayer(playersIt);
  }
}

void Game::drawCard(PlayerId playerId)
{
  if (not table.isDeckEmpty())
  {
    auto card = table.drawCard();
    hands[playerId].putCard(card);
  }
}

std::map<PlayerId, Cards> Game::getOtherPlayerHands(
  const PlayerId playerId) const
{
  std::map<PlayerId, Cards> otherPlayersHands;
  std::transform(hands.begin(),
                 hands.end(),
                 std::inserter(otherPlayersHands, otherPlayersHands.begin()),
                 [](const auto& pair) -> std::pair<PlayerId, Cards> {
                   return { pair.first, pair.second.getCards() };
                 });
  otherPlayersHands.erase(playerId);
  return otherPlayersHands;
}

void Game::runPlayerTurn(Player& player)
{
  auto playerId = player.getId();
  auto otherPlayersHands = getOtherPlayerHands(playerId);
  TurnImpl turn{
    hands.at(playerId).getIds(),
    otherPlayersHands,
    table.getGraveyard(),
    table.getStacks(),
    table.getNumberOfHints(),
    table.getNumberOfLives(),
    std::bind(
      &Game::passColorHint, this, std::placeholders::_1, std::placeholders::_2),
    std::bind(
      &Game::passValueHint, this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&Game::playCard, this, playerId, std::placeholders::_1),
    std::bind(&Game::discard, this, playerId, std::placeholders::_1)
  };
  player.playTurn(turn);
}

void Game::passColorHint(PlayerId playerId, Color color)
{
  auto [player, ids] = prepareHint(playerId, table.getColorPredicate(color));
  (*player)->takeHint(ids, color);
}

void Game::passValueHint(PlayerId playerId, Value value)
{
  auto [player, ids] = prepareHint(playerId, table.getValuePredicate(value));
  (*player)->takeHint(ids, value);
}

std::tuple<Players::const_iterator, std::list<CardId>> Game::prepareHint(
  PlayerId playerId,
  std::function<bool(const Card&)> predicate)
{
  table.useHintToken();
  auto player = getPlayerById(playerId);

  Hand hand = hands.at(playerId);
  std::list<CardId> ids = hand.getIds(predicate);
  return { player, ids };
}

Players::const_iterator Game::getPlayerById(PlayerId playerId) const
{
  auto player =
    std::find_if(players.begin(), players.end(), [playerId](auto player) {
      return player->getId() == playerId;
    });
  if (players.end() == player)
  {
    throw NoSuchPlayerException();
  }
  return player;
}

Card Game::getCard(PlayerId playerId, CardId cardId)
{
  Hand& hand = hands.at(playerId);
  return hand.getCard(cardId);
}

void Game::playCard(PlayerId currentPlayerId, CardId cardId)
{
  auto card = getCard(currentPlayerId, cardId);
  table.playCard(card);
  drawCard(currentPlayerId);
}

void Game::discard(PlayerId currentPlayerId, CardId cardId)
{
  auto card = getCard(currentPlayerId, cardId);
  table.discard(card);
  drawCard(currentPlayerId);
}