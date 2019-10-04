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

class NoSuchPlayerException : public std::exception
{
};

using Players = std::list<std::shared_ptr<Player>>;

class Game
{
  void dealCards();
  void runPlayerTurn(Player& player);
  void validate();
  void play();
  void passHint(PlayerId, Color);

  Players players;
  Cards deck;
  std::map<PlayerId, Cards> hands;
  int numberOfHints;

public:
  Game(Game&) = delete;
  Game(Players& players, Cards);
};
