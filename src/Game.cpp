#include "Game.hpp"
#include <algorithm>
#include <functional>

class TurnImpl : public Turn
{
  std::function<void(PlayerId, Color)> giveColorHintImpl;
  std::function<void(PlayerId, Value)> giveValueHintImpl;
  std::function<void(CardId)> playCardImpl;

public:
  TurnImpl(std::list<CardId> playerHand,
           std::map<PlayerId, Cards> otherPlayers,
           Cards graveyard,
           int numberOfHints,
           std::function<void(PlayerId, Color)> giveColorHintImpl,
           std::function<void(PlayerId, Value)> giveValueHintImpl,
           std::function<void(CardId)> playCardImpl)
    : Turn{ playerHand, otherPlayers, graveyard, numberOfHints }
    , giveColorHintImpl(giveColorHintImpl)
    , giveValueHintImpl(giveValueHintImpl)
    , playCardImpl(playCardImpl)
  {
  }

  void giveHint(PlayerId playerId, Color color)
  {
    giveColorHintImpl(playerId, color);
  };

  void giveHint(PlayerId playerId, Value value)
  {
    giveValueHintImpl(playerId, value);
  };
  void playCard(CardId cardId) { playCardImpl(cardId); };
  void discard(CardId){};
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

Game::Game(Players& players, std::list<Card> deck)
  : players(players)
  , deck(deck)
  , hands()
  , numberOfHints(8)
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
  runPlayerTurn(*players.front());
  runPlayerTurn(*players.back());
}

void Game::dealCards()
{
  auto it = deck.begin();
  auto playersIt = players.begin();
  for (size_t i = 0; i < 10; ++i)
  {
    hands[(*playersIt)->getId()].push_back(*it);
    it++;

    playersIt++;
    if (playersIt == players.end())
      playersIt = players.begin();
  }
}

void Game::runPlayerTurn(Player& player)
{
  auto playerId = player.getId();
  TurnImpl turn{
    transformToCardIds(hands, playerId),
    std::map<PlayerId, Cards>{
      { players.back()->getId(), hands[players.back()->getId()] } },
    {},
    numberOfHints,
    std::bind(
      &Game::passColorHint, this, std::placeholders::_1, std::placeholders::_2),
    std::bind(
      &Game::passValueHint, this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&Game::playCard, this, std::placeholders::_1)

  };
  player.playTurn(turn);
}

void Game::passColorHint(PlayerId playerId, Color color)
{
  auto [player, ids] = prepareHint(
    playerId, [color](const Card& card) { return card.color == color; });
  (*player)->takeHint(ids, color);
}

void Game::passValueHint(PlayerId playerId, Value value)
{
  auto [player, ids] = prepareHint(
    playerId, [value](const Card& card) { return card.value == value; });
  (*player)->takeHint(ids, value);
}

std::tuple<Players::iterator, std::list<CardId>> Game::prepareHint(
  PlayerId playerId,
  std::function<bool(const Card&)> predicate)
{
  --numberOfHints;
  auto player = getPlayerById(playerId);

  Cards hand = hands.at(playerId);
  std::list<CardId> ids;
  for (auto card : hand)
  {
    if (predicate(card))
    {
      ids.push_back(card.id);
    }
  }
  return { player, ids };
}

Players::iterator Game::getPlayerById(PlayerId playerId)
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

void Game::playCard(CardId)
{
  throw CardNotInHandException();
}