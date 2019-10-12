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

class YellowStackBuiltUpToTwoAndYellowThreeDiscardedTableTests
  : public ::testing::Test
{
public:
  YellowStackBuiltUpToTwoAndYellowThreeDiscardedTableTests()
    : table({
        { 0, Color::YELLOW, Value::ONE },
        { 1, Color::YELLOW, Value::ONE },
        { 2, Color::YELLOW, Value::TWO },
        { 3, Color::YELLOW, Value::FOUR },
        { 4, Color::YELLOW, Value::FIVE },
      })
  {
    table.playCard({ 5, Color::YELLOW, Value::ONE });
    table.playCard({ 6, Color::YELLOW, Value::TWO });
    table.discard({ 7, Color::YELLOW, Value::THREE });
  }
  Table table;
};

TEST_F(YellowStackBuiltUpToTwoAndYellowThreeDiscardedTableTests,
       WhenSecondYellowThreeDiscardedThenIsWinnableIsFalse)
{
  table.discard({ 8, Color::YELLOW, Value::THREE });
  EXPECT_FALSE(table.isWinnable());
}

TEST_F(YellowStackBuiltUpToTwoAndYellowThreeDiscardedTableTests,
       WhenSecondYellowThreePlayedThenIsWinnableIsTrue)
{
  table.playCard({ 8, Color::YELLOW, Value::THREE });
  EXPECT_TRUE(table.isWinnable());
}

// TEST_F()