#include "Game.hpp"
#include <algorithm>

Game::TurnImpl::TurnImpl(std::list<CardId> playerHand,
                         std::map<PlayerId, Hand> otherPlayers,
                         int numberOfHints)
  : Turn{ playerHand, otherPlayers, numberOfHints }
{
}

void Game::TurnImpl::discard(CardId) {}
void Game::TurnImpl::giveHint(PlayerId, Color) {}
void Game::TurnImpl::giveHint(PlayerId, Value) {}
void Game::TurnImpl::playCard(CardId) {}

Game::Game(Players& players, std::list<Card> deck)
{
  if (players.size() < 2)
    throw NotEnoughPlayersException();
  if (players.size() > 5)
    throw TooManyPlayersException();
  Hand player1Hand;
  Hand player2Hand;
  std::map<PlayerId, Hand> hands;
  for (auto player : players)
  {
    hands.insert({ player->getId(), {} });
  }
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

  std::list<CardId> cardIds;
  std::transform(hands[players.front()->getId()].begin(),
                 hands[players.front()->getId()].end(),
                 std::back_inserter(cardIds),
                 [](const Card& card) { return card.id; });
  TurnImpl turn{ cardIds,
                 std::map<PlayerId, Hand>{ { players.back()->getId(),
                                             hands[players.back()->getId()] } },
                 8 };
  players.front()->playTurn(turn);
}
