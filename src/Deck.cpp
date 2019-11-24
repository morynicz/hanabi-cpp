#include "Deck.hpp"
#include <iostream>

Deck::Deck(const Composition& composition)
  : composition(composition)
{
  CardId id = 0;
  for (auto [color, values] : composition)
  {
    for (auto [value, number] : values)
    {
      for (int i = 0; i < number; ++i)
        cards.push_back(Card{ id++, color, value });
    }
  }
}

Deck::Deck(const std::list<Card>& cards)
  : cards(cards)
{
  for (auto card : cards)
  {
    auto& color = composition[card.color];
    auto& value = color[card.value];
    value++;
  }
}

Card Deck::draw()
{
  auto card = cards.front();
  cards.pop_front();
  return card;
}

Composition Deck::getComposition() const
{
  return composition;
}
