#include "Table.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

class TableTests : public ::testing::Test
{
};

TEST(TableTests, WhenCardsFrom1To5ArePlayedThenNoLivesAreLost)
{
  Table table({});
  table.playCard({ 0, Color::BLUE, Value::ONE });
  table.playCard({ 0, Color::BLUE, Value::TWO });
  table.playCard({ 0, Color::BLUE, Value::THREE });
  table.playCard({ 0, Color::BLUE, Value::FOUR });
  table.playCard({ 0, Color::BLUE, Value::FIVE });
  EXPECT_EQ(table.getNumberOfLives(), MAX_LIVES);
}