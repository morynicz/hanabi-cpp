#include "Game.hpp"
#include <algorithm>

Game::TurnImpl::TurnImpl(std::list<CardId> playerHand,
                         std::map<PlayerId, Cards> otherPlayers,
                         Cards graveyard,
                         int numberOfHints)
  : Turn{ playerHand, otherPlayers, graveyard, numberOfHints }
{
}

void Game::TurnImpl::discard(CardId) {}
void Game::TurnImpl::giveHint(PlayerId, Color) {}
void Game::TurnImpl::giveHint(PlayerId, Value) {}
void Game::TurnImpl::playCard(CardId) {}

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
  TurnImpl turn{ getCardIds(hands, playerId),
                 std::map<PlayerId, Cards>{
                   { players.back()->getId(),
                     hands[players.back()->getId()] } },
                 {},
                 8 };
  player.playTurn(turn);
}