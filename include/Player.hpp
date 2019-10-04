#pragma once

#include "Ids.hpp"
#include "Turn.hpp"

class Player
{
public:
  Player() = default;
  virtual PlayerId getId() const = 0;
  virtual void playTurn(Turn&) = 0;
  virtual void takeHint(std::list<CardId>, Color) = 0;
  virtual void takeHint(std::list<CardId>, Value) = 0;
  virtual ~Player() = default;
};