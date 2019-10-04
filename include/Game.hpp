#pragma once
#include "Card.hpp"
#include "Ids.hpp"
#include "Player.hpp"
#include <functional>
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

class CardNotInHandException : public std::exception
{
};

using Players = std::list<std::shared_ptr<Player>>;

class Game
{
  void dealCards();
  void runPlayerTurn(Player& player);
  void validate();
  void play();
  void passColorHint(PlayerId, Color);
  void passValueHint(PlayerId, Value);
  std::tuple<Players::iterator, std::list<CardId>> prepareHint(
    PlayerId player,
    std::function<bool(const Card&)> predicate);
  Players::iterator getPlayerById(PlayerId);
  void playCard(CardId);

  Players players;
  Cards deck;
  std::map<PlayerId, Cards> hands;
  unsigned char numberOfHints;

public:
  Game(Game&) = delete;
  Game(Players& players, Cards);
};
