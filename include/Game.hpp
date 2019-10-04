#pragma once
#include "Card.hpp"
#include "Ids.hpp"
#include "Player.hpp"
#include <list>
#include <memory>
#include <stdexcept>

class NotEnoughPlayersException : public std::exception
{
};
class TooManyPlayersException : public std::exception
{
};

using Players = std::list<std::shared_ptr<Player>>;

class Game
{
  class TurnImpl : public Turn
  {
  public:
    TurnImpl(std::list<CardId>,
             std::map<PlayerId, std::list<Card>>,
             Cards graveyard,
             int);
    void giveHint(PlayerId, Color);
    void giveHint(PlayerId, Value);
    void playCard(CardId);
    void discard(CardId);
    virtual ~TurnImpl() = default;
  };

  void dealCards();
  void runPlayerTurn(Player& player);
  void validate();
  void play();

  Players players;
  Cards deck;
  std::map<PlayerId, Cards> hands;

public:
  Game(Game&) = delete;
  Game(Players& players, Cards);
};
