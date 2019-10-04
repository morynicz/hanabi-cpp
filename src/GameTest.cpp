#include "Game.hpp"
#include "Player.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

bool operator==(const Card& lhs, const Card& rhs)
{
  return lhs.id == rhs.id;
}

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

constexpr PlayerId PLAYER_1_ID = 2;
constexpr PlayerId PLAYER_2_ID = 4;

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
             { 9, Color::YELLOW, Value::FOUR },
             { 10, Color::RED, Value::ONE } })
    , players{ player1, player2 }
  {
    EXPECT_CALL(*player1, getId())
      .WillRepeatedly(::testing::Return(PLAYER_1_ID));
    EXPECT_CALL(*player2, getId())
      .WillRepeatedly(::testing::Return(PLAYER_2_ID));
  }

  std::shared_ptr<MockPlayer> player1 =
    std::make_shared<::testing::StrictMock<MockPlayer>>();
  std::shared_ptr<MockPlayer> player2 =
    std::make_shared<::testing::StrictMock<MockPlayer>>();
  Players players;
  std::vector<Card> deck;
};

TEST_F(
  TwoPlayerGameTests,
  GivenTwoPlayerGameWhenGameStartsThenPlayerOneGetsTurnWith5CardsAnd8AvailableHints)
{
  EXPECT_CALL(
    *player1,
    playTurn(::testing::AllOf(
      ::testing::Field(&Turn::numberOfHints, 8),
      ::testing::Field(
        &Turn::playerHand,
        ::testing::ElementsAre(
          deck[0].id, deck[2].id, deck[4].id, deck[6].id, deck[8].id)),
      ::testing::Field(
        &Turn::otherPlayers,
        ::testing::ElementsAre(std::pair<PlayerId, Hand>{
          PLAYER_2_ID,
          Hand{ deck[1], deck[3], deck[5], deck[7], deck[9] } })))));
  Game game(players, std::list<Card>(deck.begin(), deck.end()));
}