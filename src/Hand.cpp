#include "Hand.hpp"
#include <algorithm>

Card Hand::getCard(const CardId cardId)
{
  auto card =
    std::find_if(cards.begin(), cards.end(), [cardId](const Card& card) {
      return card.id == cardId;
    });
  if (card == cards.end())
    throw CardNotInHandException();
  auto copy = *card;
  cards.erase(card);
  return copy;
}

std::list<CardId> Hand::getIds(std::function<bool(const Card&)> predicate)
{
  std::list<CardId> ids;
  for (auto card : cards)
  {
    if (predicate(card))
    {
      ids.push_back(card.id);
    }
  }
  return ids;
}

void Hand::putCard(const Card& card)
{
  cards.push_back(card);
}