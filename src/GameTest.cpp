#include "Game.hpp"
#include "Player.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

class MockPlayer : public Player
{
public:
  MOCK_CONST_METHOD0(getId, PlayerId());
  MOCK_METHOD1(playTurn, void(Turn&));
};

class DummyPlayer : public Player
{
  PlayerId getId() const { return 0; }
  void playTurn(Turn&) {}
};

TEST(Basic, WhenPreparingGameFor1PlayerThenThrowNotEnoughPlayersException)
{
  Players players;
  players.push_back(std::make_unique<DummyPlayer>());
  EXPECT_THROW({ Game game(players, {}); }, NotEnoughPlayersException);
}

TEST(Basic, WhenPreparingGameFor6PlayersThenThrowTooManyPlayersException)
{
  Players players;
  players.push_back(std::make_unique<DummyPlayer>());
  players.push_back(std::make_unique<DummyPlayer>());
  players.push_back(std::make_unique<DummyPlayer>());
  players.push_back(std::make_unique<DummyPlayer>());
  players.push_back(std::make_unique<DummyPlayer>());
  players.push_back(std::make_unique<DummyPlayer>());

  EXPECT_THROW({ Game game(players, {}); }, TooManyPlayersException);
}

struct TwoPlayerGameTests : public ::testing::Test
{
  TwoPlayerGameTests()
    : deck({ { 0, Color::BLUE, Value::FOUR },
             { 1, Color::RED, Value::ONE },
             { 2, Color::WHITE, Value::THREE },
             { 3, Color::BLUE, Value::ONE },
             { 4, Color::WHITE, Value::TWO },
             { 5, Color::YELLOW, Value::FIVE },
             { 6, Color::RED, Value::FOUR },
             { 7, Color::WHITE, Value::ONE },
             { 8, Color::BLUE, Value::TWO },
             { 9, Color::YELLOW, Value::FOUR } })
    , players{ player1, player2 }
  {
  }

  Players players;
  std::shared_ptr<MockPlayer> player1 = std::make_shared<MockPlayer>();
  std::shared_ptr<MockPlayer> player2 = std::make_shared<MockPlayer>();
  std::list<Card> deck;
};

TEST_F(
  TwoPlayerGameTests,
  GivenTwoPlayerGameWhenGameStartsThenPlayerOneGetsTurnWith5CardsAnd8AvailableHints)
{
  EXPECT_CALL(*player1, playTurn(::testing::_));
}