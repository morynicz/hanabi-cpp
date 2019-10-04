#include "Game.hpp"
#include "Player.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockPlayer : public Player
{
};

TEST(Basic, WhenPreparingGameFor1PlayerThenThrowNotEnoughPlayersException)
{
  EXPECT_THROW({ Game game(std::list<Player>{ Player{} }); },
               NotEnoughPlayersException);
}

TEST(Basic, WhenPreparingGameFor6PlayersThenThrowTooManyPlayersException)
{
  EXPECT_THROW(
    {
      Game game(std::list<Player>{ {}, {}, {}, {}, {}, {} });
    },
    TooManyPlayersException);
}

struct TwoPlayerGameTests
{
  TwoPlayerGameTests()
    : player1()
    , player2()
    , game(std::list<Player>{ player1, player2 })
  {
  }

  MockPlayer player1;
  MockPlayer player2;
  Game game;
};