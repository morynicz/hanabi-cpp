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
public:
  Game(Game&) = delete;
  Game(Players& players, std::list<Card>)
  {
    if (players.size() < 2)
      throw NotEnoughPlayersException();
    if (players.size() > 5)
      throw TooManyPlayersException();
  }
};
