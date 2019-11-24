#include "Deck.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

static bool operator==(const Card& lhs, const Card& rhs)
{
  return lhs.color == rhs.color and lhs.value == rhs.value;
}

TEST(Deck_tests, Deck_contains_cards_with_specified_composition)
{
  Deck deck(BASIC_DECK_COMPOSITION);
  std::list<Card> cards;
  while (not deck.isEmpty())
  {
    cards.push_back(deck.draw());
  }
  EXPECT_THAT(cards, testing::UnorderedElementsAreArray(FULL_BASIC_DECK));
}

TEST(Deck_tests, given_list_of_cards_deck_returns_correct_composition)
{
  Deck deck(FULL_BASIC_DECK);
  EXPECT_EQ(deck.getComposition(), BASIC_DECK_COMPOSITION);
}