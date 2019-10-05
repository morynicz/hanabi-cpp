#include "Table.hpp"

void Table::discard(const Card& card)
{
  graveyard.push_back(card);
  if (numberOfHints < MAX_HINTS)
    ++numberOfHints;
}

bool Table::isOver() const
{
  return numberOfLives <= 0;
}
Card Table::drawCard()
{
  auto card = deck.front();
  deck.pop_front();
  return card;
}
std::function<bool(const Card&)> Table::getColorPredicate(
  const Color color) const
{
  return [color](const Card& card) { return card.color == color; };
}

std::function<bool(const Card&)> Table::getValuePredicate(
  const Value value) const
{
  return [value](const Card& card) { return card.value == value; };
}
void Table::useHintToken()
{
  if (0 == numberOfHints)
    throw NoMoreHintsAvailableException();
  --numberOfHints;
}
void Table::playCard(const Card& card)
{
  if (isOpeningNewStack(card))
  {
    stacks.insert_or_assign(card.color, card.value);
  }
  else if (isStackOpened(card.color) and
           areAdjacentAscending(stacks[card.color], card.value))
  {
    stacks.insert_or_assign(card.color, card.value);
  }
  else
  {
    graveyard.push_back(card);
    --numberOfLives;
  }
}
bool Table::isOpeningNewStack(const Card& card)
{
  return card.value == Value::ONE and not isStackOpened(card.color);
}
bool Table::isStackOpened(const Color color)
{
  return stacks.find(color) != stacks.end();
}
