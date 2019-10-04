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

constexpr unsigned char MAX_LIVES = 3;
constexpr unsigned char MAX_HINTS = 8;

class Game
{
  void dealCards();
  void runPlayerTurn(Player&);
  void validate();
  void play();
  void passColorHint(PlayerId, Color);
  void passValueHint(PlayerId, Value);
  std::tuple<Players::iterator, std::list<CardId>> prepareHint(
    PlayerId,
    std::function<bool(const Card&)>);
  Players::iterator getPlayerById(PlayerId);
  void playCard(PlayerId, CardId);
  void drawCard(PlayerId);
  void discard(PlayerId, CardId);
  bool isOpeningNewStack(const Card&);
  Card getCard(PlayerId, CardId);
  void advancePlayer(Players::iterator& playersIt);

  Players players;
  Cards deck;
  std::map<PlayerId, Cards> hands;
  std::map<Color, Value> stacks;
  Cards graveyard;
  unsigned char numberOfHints;
  unsigned char numberOfLives;
  Players::iterator currentPlayer;

public:
  Game(Game&) = delete;
  Game(Players& players, Cards);
  void turn();
};
