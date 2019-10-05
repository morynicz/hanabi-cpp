#pragma once
#include "Card.hpp"
#include "Ids.hpp"
#include <functional>
#include <list>

class CardNotInHandException : public std::exception
{
};

class Hand
{
  std::list<Card> cards;

public:
  Hand() = default;
  Card getCard(const CardId);
  void putCard(const Card&);
  std::list<CardId> getIds(std::function<bool(const Card&)> predicate =
                             [](const Card&) { return true; });
  inline std::list<Card> getCards() const { return cards; }
};