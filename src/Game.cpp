#include "Game.hpp"
#include <algorithm>
#include <functional>

class TurnImpl : public Turn
{
  std::function<void(PlayerId, Color)> giveColorHintImpl;

public:
  TurnImpl(std::list<CardId> playerHand,
           std::map<PlayerId, Cards> otherPlayers,
           Cards graveyard,
           int numberOfHints,
           std::function<void(PlayerId, Color)> giveColorHintImpl)
    : Turn{ playerHand, otherPlayers, graveyard, numberOfHints }
    , giveColorHintImpl(giveColorHintImpl)
  {
  }

  void giveHint(PlayerId playerId, Color color)
  {
    giveColorHintImpl(playerId, color);
  };

  void giveHint(PlayerId, Value){};
  void playCard(CardId){};
  void discard(CardId){};
  virtual ~TurnImpl() = default;
};

std::list<CardId> getCardIds(const std::map<PlayerId, Cards>& hands,
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
    getCardIds(hands, playerId),
    std::map<PlayerId, Cards>{
      { players.back()->getId(), hands[players.back()->getId()] } },
    {},
    8,
    [this](PlayerId playerId, Color color) {
      Cards hand = hands.at(playerId);
      std::list<CardId> ids;
      for (auto card : hand)
      {
        if (card.color == color)
        {
          ids.push_back(card.id);
        }
      }
      auto player =
        std::find_if(players.begin(), players.end(), [playerId](auto player) {
          return player->getId() == playerId;
        });
      (*player)->takeHint(ids, color);
    }
  };
  player.playTurn(turn);
}