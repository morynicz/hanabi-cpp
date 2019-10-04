#include "Game.hpp"
#include "Player.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using ::testing::ElementsAre;
using ::testing::Field;
using ::testing::Return;

bool operator==(const Card& lhs, const Card& rhs)
{
  return lhs.id == rhs.id;
}

class MockPlayer : public Player
{
public:
  MOCK_CONST_METHOD0(getId, PlayerId());
  MOCK_METHOD1(playTurn, void(Turn&));
  MOCK_METHOD2(takeHint, void(std::list<CardId>, Color));
  MOCK_METHOD2(takeHint, void(std::list<CardId>, Value));
};

class DummyPlayer : public Player
{
  PlayerId getId() const { return 0; }
  void playTurn(Turn&) {}
  void takeHint(std::list<CardId>, Color){};
  void takeHint(std::list<CardId>, Value){};
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
    EXPECT_CALL(*player1, getId()).WillRepeatedly(Return(PLAYER_1_ID));
    EXPECT_CALL(*player2, getId()).WillRepeatedly(Return(PLAYER_2_ID));
  }

  std::vector<Card> deck;
  std::shared_ptr<MockPlayer> player1 =
    std::make_shared<::testing::NiceMock<MockPlayer>>();
  std::shared_ptr<MockPlayer> player2 =
    std::make_shared<::testing::NiceMock<MockPlayer>>();
  Players players;
};

TEST_F(TwoPlayerGameTests,
       WhenGameStartsThenPlayerOneGetsTurnWith5CardsAnd8AvailableHints)
{
  EXPECT_CALL(
    *player1,
    playTurn(::testing::AllOf(
      Field(&Turn::numberOfHints, 8),
      Field(&Turn::playerHand,
            ElementsAre(
              deck[0].id, deck[2].id, deck[4].id, deck[6].id, deck[8].id)),
      Field(
        &Turn::otherPlayers,
        ElementsAre(std::pair<PlayerId, Cards>{
          PLAYER_2_ID, Cards{ deck[1], deck[3], deck[5], deck[7], deck[9] } })),
      Field(&Turn::graveyard, ::testing::IsEmpty()))));
  Game game(players, std::list<Card>(deck.begin(), deck.end()));
}

ACTION_P2(PlayerGiveHint, playerId, hint)
{
  arg0.giveHint(playerId, hint);
}

TEST_F(
  TwoPlayerGameTests,
  GivenFirstTurnWhenPlayer1GivesColorHintToPlayer2ThenTurnGivenToPlayer2Has7HintsAndSelectedCardsHaveColorRevealed)
{
  EXPECT_CALL(*player1, playTurn(::testing::_))
    .WillOnce(PlayerGiveHint(PLAYER_2_ID, Color::RED));
  EXPECT_CALL(*player2, takeHint(std::list<CardId>{ 1 }, Color::RED));
  Game game(players, std::list<Card>(deck.begin(), deck.end()));
}