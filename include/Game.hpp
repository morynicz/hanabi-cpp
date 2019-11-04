#pragma once
#include "Card.hpp"
#include "Hand.hpp"
#include "Ids.hpp"
#include "Player.hpp"
#include "Table.hpp"
#include <functional>
#include <list>
#include <memory>
#include <stdexcept>

class NotEnoughPlayersException : public std::exception
{
  const char* what() const noexcept override { return "Not enough players"; }
};

class TooManyPlayersException : public std::exception
{
  const char* what() const noexcept override { return "Too many players"; }
};

class NoSuchPlayerException : public std::exception
{
  const char* what() const noexcept override { return "No such player"; }
};

class GameIsOverException : public std::exception
{
  const char* what() const noexcept override { return "Game is over"; }
};

using Players = std::list<std::shared_ptr<Player>>;

class Game
{
  void dealCards();
  void runPlayerTurn(Player&);
  void validate();
  void play();
  void passColorHint(PlayerId, Color);
  void passValueHint(PlayerId, Value);
  std::tuple<Players::const_iterator, std::list<CardId>> prepareHint(
    PlayerId,
    std::function<bool(const Card&)>);
  Players::const_iterator getPlayerById(PlayerId) const;
  void playCard(PlayerId, CardId);
  void drawCard(PlayerId);
  void discard(PlayerId, CardId);
  Card getCard(PlayerId, CardId);
  void advancePlayer(Players::const_iterator& playersIt);
  std::map<PlayerId, Cards> getOtherPlayerHands(const PlayerId) const;
  bool isEndGame() const;
  bool isGameOver() const;

  Players players;
  Table table;
  std::map<PlayerId, Hand> hands;
  Players::const_iterator currentPlayer;
  int turnsLeftCounter;

public:
  Game(const Game&) = delete;
  Game(Game&&) = delete;
  Game(const Players& players, Cards);
  void turn();
  int getScore() const;
};
