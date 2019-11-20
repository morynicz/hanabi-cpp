#include "Game.hpp"
#include <algorithm>
#include <functional>

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

Game::Game(const Players& players, const std::list<Card>& deck)
  : players(players)
  , table{ deck }
  , hands()
  , currentPlayer(Game::players.cbegin())
  , turnsLeftCounter(Game::players.size())
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
  for (auto player : players)
  {
    hands.insert({ player->getId(), {} });
  }
  dealCards();
}

bool Game::isEndGame() const
{
  return !table.isWinnable() and table.isDeckEmpty();
}

bool Game::isGameOver() const
{
  return table.isOver() or 0 > turnsLeftCounter;
}

void Game::turn()
{
  if (isEndGame())
  {
    --turnsLeftCounter;
  }

  if (isGameOver())
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
  Turn turn{
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
  (*player)->takeHint(playerId, ids, color);
}

void Game::passValueHint(PlayerId playerId, Value value)
{
  auto [player, ids] = prepareHint(playerId, table.getValuePredicate(value));
  (*player)->takeHint(playerId, ids, value);
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

int Game::getScore() const
{
  return table.getScore();
}