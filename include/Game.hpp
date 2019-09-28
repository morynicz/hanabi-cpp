#pragma once
#include "Player.hpp"
#include <list>
#include <stdexcept>

class NotEnoughPlayersException : public std::exception
{
};
class TooManyPlayersException : public std::exception
{
};

class Game
{
public:
  Game(Game&) = delete;
  Game(std::list<Player> players)
  {
    if (players.size() < 2)
      throw NotEnoughPlayersException();
    if (players.size() > 5)
      throw TooManyPlayersException();
  }
};
