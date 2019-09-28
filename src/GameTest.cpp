#include "Game.hpp"
#include "Player.hpp"
#include <gtest/gtest.h>

TEST(Basic, WhenPreparingGameFor1PlayerThenThrowNotEnoughPlayersException)
{
  EXPECT_THROW({ Game game(std::list<Player>{ Player{} }); },
               NotEnoughPlayersException);
}

TEST(Basic,
     WhenPreparingGameFor6PlayersThenThrowTooManyPlayersException)
{
  EXPECT_THROW({ Game game(std::list<Player>{ {}, {},{},{},{},{} }); },
               TooManyPlayersException);
}