#include "Table.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

struct BlueStackBuiltUpToFourTableTests : public ::testing::Test
{
  BlueStackBuiltUpToFourTableTests()
    : table({})
  {
    table.playCard({ 0, Color::BLUE, Value::ONE });
    table.playCard({ 0, Color::BLUE, Value::TWO });
    table.playCard({ 0, Color::BLUE, Value::THREE });
    table.playCard({ 0, Color::BLUE, Value::FOUR });
  }

  Table table;
};

TEST_F(BlueStackBuiltUpToFourTableTests,
       WhenCardsFrom1To5ArePlayedThenNoLivesAreLost)
{
  table.playCard({ 0, Color::BLUE, Value::FIVE });
  EXPECT_EQ(table.getNumberOfLives(), MAX_LIVES);
}

TEST_F(
  BlueStackBuiltUpToFourTableTests,
  GivenOneHintIsUsedUpAndStackIsBuiltUpTo4When5ISPutOnStackThenHintIsRestored)
{
  table.useHintToken();

  table.playCard({ 0, Color::BLUE, Value::FIVE });

  EXPECT_EQ(table.getNumberOfHints(), MAX_HINTS);
}

TEST_F(
  BlueStackBuiltUpToFourTableTests,
  GivenNoHintIsUsedUpAndStackIsBuiltUpTo4When5ISPutOnStackThenHintIsRestored)
{
  table.playCard({ 0, Color::BLUE, Value::FIVE });

  EXPECT_EQ(table.getNumberOfHints(), MAX_HINTS);
}