#include "Table.hpp"

void Table::discard(const Card& card)
{
  putToGraveyard(card);
  restoreHint();
}

bool Table::isOver() const
{
  return numberOfLives <= 0 or areAllStacksFinished();
}

bool Table::areAllStacksFinished() const
{
  return (stacks.size() == NUMBER_OF_COLORS and
          std::all_of(stacks.begin(), stacks.end(), [](const auto& top) {
            return top.second == Value::FIVE;
          }));
}

Card Table::drawCard()
{
  return deck.draw();
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
  else if (isAddingCardToStack(card))
  {
    stacks.insert_or_assign(card.color, card.value);

    if (card.value == Value::FIVE)
    {
      restoreHint();
    }
  }
  else
  {
    putToGraveyard(card);
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

bool Table::isAddingCardToStack(const Card& card)
{
  return isStackOpened(card.color) and
         areAdjacentAscending(stacks[card.color], card.value);
}

void Table::restoreHint()
{
  if (numberOfHints < MAX_HINTS)
    ++numberOfHints;
}

bool Table::isWinnable() const
{
  return std::none_of(
    winnableCondition.begin(),
    winnableCondition.end(),
    [](const std::pair<Color, std::unordered_map<Value, int>>& values) {
      return any_of(values.second.begin(),
                    values.second.end(),
                    [](const std::pair<Value, int>& numberOfLeft) {
                      return numberOfLeft.second <= 0;
                    });
    });
}

void Table::putToGraveyard(const Card& card)
{
  graveyard.push_back(card);
  winnableCondition.at(card.color).at(card.value)--;
}

int Table::getScore() const
{
  int score = 0;
  for (auto stack : stacks)
  {
    score += static_cast<int>(stack.second);
  }
  return score;
}