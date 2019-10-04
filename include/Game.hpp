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
    TurnImpl(std::list<CardId>, std::map<PlayerId, std::list<Card>>, int);
    void giveHint(PlayerId, Color);
    void giveHint(PlayerId, Value);
    void playCard(CardId);
    void discard(CardId);
    virtual ~TurnImpl() = default;
  };

public:
  Game(Game&) = delete;
  Game(Players& players, std::list<Card>);
};
