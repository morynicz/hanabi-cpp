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
  TurnImpl(std::list<CardId> playerHand,
           std::map<PlayerId, Cards> otherPlayers,
           Cards graveyard,
           std::map<Color, Value> stacks,
           unsigned char numberOfHints,
           unsigned char numberOfLives,
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
  , numberOfHints(MAX_HINTS)
  , numberOfLives(MAX_LIVES)
  , currentPlayer(players.begin())
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
  currentPlayer = players.begin();
}

void Game::turn()
{
  runPlayerTurn(**currentPlayer);
  advancePlayer(currentPlayer);
}

void Game::advancePlayer(Players::iterator& playersIt)
{
  playersIt++;
  if (playersIt == players.end())
    playersIt = players.begin();
}

void Game::dealCards()
{
  auto playersIt = players.begin();
  for (size_t i = 0; i < 10; ++i)
  {
    drawCard((*playersIt)->getId());
    advancePlayer(playersIt);
  }
}

void Game::drawCard(PlayerId playerId)
{
  hands[playerId].push_back(deck.front());
  deck.pop_front();
}

void Game::runPlayerTurn(Player& player)
{
  auto playerId = player.getId();
  auto otherPlayersHands(hands);
  otherPlayersHands.erase(playerId);
  TurnImpl turn{
    transformToCardIds(hands, playerId),
    otherPlayersHands,
    graveyard,
    stacks,
    numberOfHints,
    numberOfLives,
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

Card Game::getCard(PlayerId playerId, CardId cardId)
{
  Cards& hand = hands.at(playerId);
  auto card =
    std::find_if(hand.begin(), hand.end(), [cardId](const Card& card) {
      return card.id == cardId;
    });
  if (card == hand.end())
    throw CardNotInHandException();
  hand.erase(card);
  return *card;
}

void Game::playCard(PlayerId currentPlayerId, CardId cardId)
{
  auto card = getCard(currentPlayerId, cardId);
  if (isOpeningNewStack(card))
  {
    stacks.insert_or_assign(card.color, card.value);
  }
  else
  {
    graveyard.push_back(card);
    --numberOfLives;
  }
  drawCard(currentPlayerId);
}

bool Game::isOpeningNewStack(const Card& card)
{
  return card.value == Value::ONE and stacks.find(card.color) == stacks.end();
}

void Game::discard(PlayerId currentPlayerId, CardId cardId)
{
  auto card = getCard(currentPlayerId, cardId);
  graveyard.push_back(card);
  if (numberOfHints < MAX_HINTS)
    ++numberOfHints;
  drawCard(currentPlayerId);
}