#include "Game.hpp"
#include "Player.hpp"
#include "TestPrinters.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

using ::testing::ElementsAre;
using ::testing::Field;
using ::testing::Return;

static bool operator==(const Card& lhs, const Card& rhs)
{
  return lhs.id == rhs.id;
}

std::ostream& operator<<(std::ostream& str, const Turn& turn)
{
  return str << "playerHand: " << turn.playerHand
             << "; otherPlayers: " << turn.otherPlayers
             << "; graveyard: " << turn.graveyard << "; stacks: " << turn.stacks
             << "; numberOfHints: "
             << static_cast<unsigned int>(turn.numberOfHints)
             << "; numberOfLives: "
             << static_cast<unsigned int>(turn.numberOfLives);
}

class MockPlayer : public Player
{
public:
  MOCK_METHOD(PlayerId, getId, (), (const, override));
  MOCK_METHOD(void, playTurn, (Turn&), (override));
  MOCK_METHOD(void, takeHint, (PlayerId, std::list<CardId>, Color), (override));
  MOCK_METHOD(void, takeHint, (PlayerId, std::list<CardId>, Value), (override));
};

class DummyPlayer : public Player
{
  PlayerId getId() const { return 0; }
  void playTurn(Turn&) {}
  void takeHint(PlayerId, std::list<CardId>, Color){};
  void takeHint(PlayerId, std::list<CardId>, Value){};
};

constexpr PlayerId UNKNOWN_PLAYER_ID = 333;
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
    : deck({ { 0, Color::BLUE, Value::ONE },
             { 1, Color::RED, Value::ONE },
             { 2, Color::WHITE, Value::THREE },
             { 3, Color::BLUE, Value::ONE },
             { 4, Color::WHITE, Value::FIVE },
             { 5, Color::YELLOW, Value::TWO },
             { 6, Color::RED, Value::FOUR },
             { 7, Color::WHITE, Value::ONE },
             { 8, Color::YELLOW, Value::FOUR },
             { 9, Color::BLUE, Value::TWO },
             { 10, Color::RED, Value::ONE } })
    , players{ player1, player2 }
    , game()
  {
    EXPECT_CALL(*player1, getId()).WillRepeatedly(Return(PLAYER_1_ID));
    EXPECT_CALL(*player2, getId()).WillRepeatedly(Return(PLAYER_2_ID));
    game = std::make_unique<Game>(players,
                                  std::list<Card>(deck.begin(), deck.end()));
  }

  std::vector<Card> deck;
  std::shared_ptr<MockPlayer> player1 =
    std::make_shared<::testing::NiceMock<MockPlayer>>();
  std::shared_ptr<MockPlayer> player2 =
    std::make_shared<::testing::NiceMock<MockPlayer>>();
  Players players;
  std::unique_ptr<Game> game;
};

TEST_F(TwoPlayerGameTests,
       WhenGameStartsThenPlayerOneGetsTurnWith5CardsAnd8AvailableHints)
{
  EXPECT_CALL(
    *player1,
    playTurn(::testing::AllOf(
      Field(&Turn::numberOfHints, 8),
      Field(&Turn::numberOfLives, 3),
      Field(&Turn::playerHand,
            ElementsAre(
              deck[0].id, deck[2].id, deck[4].id, deck[6].id, deck[8].id)),
      Field(
        &Turn::otherPlayers,
        ElementsAre(std::pair<PlayerId, Cards>{
          PLAYER_2_ID, Cards{ deck[1], deck[3], deck[5], deck[7], deck[9] } })),
      Field(&Turn::graveyard, ::testing::IsEmpty()),
      Field(&Turn::stacks, ::testing::IsEmpty()))));
  game->turn();
}

ACTION_P2(PlayerGiveHint, playerId, hint)
{
  arg0.giveHint(playerId, hint);
}

TEST_F(
  TwoPlayerGameTests,
  GivenFirstTurnWhenPlayer1GivesColorHintToPlayer2ThenPlayer2TakesAColorHint)
{
  EXPECT_CALL(*player1, playTurn(::testing::_))
    .WillOnce(PlayerGiveHint(PLAYER_2_ID, Color::RED));
  EXPECT_CALL(*player2,
              takeHint(PLAYER_2_ID, std::list<CardId>{ 1 }, Color::RED));
  game->turn();
  game->turn();
}

TEST_F(
  TwoPlayerGameTests,
  GivenFirstTurnWhenPlayer1GivesColorHintToPlayer2ThenTurnGivenToPlayer2Has7Hints)
{
  ON_CALL(*player1, playTurn(::testing::_))
    .WillByDefault(PlayerGiveHint(PLAYER_2_ID, Color::RED));
  EXPECT_CALL(*player2, playTurn(Field(&Turn::numberOfHints, 7)));
  game->turn();
  game->turn();
}

TEST_F(
  TwoPlayerGameTests,
  GivenFirstTurnWhenPlayer1GivesColorHintToUnknownPlayerThenThrowNoSuchPlayerException)
{
  ON_CALL(*player1, playTurn(::testing::_))
    .WillByDefault(PlayerGiveHint(UNKNOWN_PLAYER_ID, Color::RED));
  EXPECT_THROW({ game->turn(); }, NoSuchPlayerException);
}

TEST_F(
  TwoPlayerGameTests,
  GivenFirstTurnWhenPlayer1GivesValueHintToPlayer2ThenAllPlayersAValueHintForPlayer2)
{
  EXPECT_CALL(*player1, playTurn(::testing::_))
    .WillOnce(PlayerGiveHint(PLAYER_2_ID, Value::ONE));
  EXPECT_CALL(*player2,
              takeHint(PLAYER_2_ID, std::list<CardId>{ 1, 3, 7 }, Value::ONE));
  EXPECT_CALL(*player1,
              takeHint(PLAYER_2_ID, std::list<CardId>{ 1, 3, 7 }, Value::ONE));
  game->turn();
  game->turn();
}

TEST_F(
  TwoPlayerGameTests,
  GivenFirstTurnWhenPlayer1GivesValueHintToPlayer2ThenTurnGivenToPlayer2Has7Hints)
{
  ON_CALL(*player1, playTurn(::testing::_))
    .WillByDefault(PlayerGiveHint(PLAYER_2_ID, Value::ONE));
  EXPECT_CALL(*player2, playTurn(Field(&Turn::numberOfHints, 7)));
  game->turn();
  game->turn();
}

TEST_F(
  TwoPlayerGameTests,
  GivenFirstTurnWhenPlayer1GivesValueHintToUnknownPlayerThenThrowNoSuchPlayerException)
{
  ON_CALL(*player1, playTurn(::testing::_))
    .WillByDefault(PlayerGiveHint(UNKNOWN_PLAYER_ID, Value::ONE));
  EXPECT_THROW({ game->turn(); }, NoSuchPlayerException);
}

ACTION_P(PlayerPlayCard, cardId)
{
  arg0.playCard(cardId);
}

TEST_F(
  TwoPlayerGameTests,
  GivenFirstTurnWhenPlayer1PlaysCardNotInHisHandThenCardNotInHandExceptionIsThrown)
{
  ON_CALL(*player1, playTurn(::testing::_)).WillByDefault(PlayerPlayCard(1000));
  EXPECT_THROW({ game->turn(); }, CardNotInHandException);
}

TEST_F(TwoPlayerGameTests,
       GivenFirstTurnWhenPlayer1PlaysInvalidCardThenLifeIsLost)
{
  ON_CALL(*player1, playTurn(::testing::_)).WillByDefault(PlayerPlayCard(2));
  EXPECT_CALL(
    *player2,
    playTurn(AllOf(
      Field(&Turn::numberOfHints, 8),
      Field(&Turn::numberOfLives, 2),
      Field(&Turn::playerHand,
            ElementsAre(
              deck[1].id, deck[3].id, deck[5].id, deck[7].id, deck[9].id)),
      Field(&Turn::otherPlayers,
            ElementsAre(std::pair<PlayerId, Cards>{
              PLAYER_1_ID,
              Cards{ deck[0], deck[4], deck[6], deck[8], deck[10] } })),
      Field(&Turn::graveyard, ElementsAre(deck[2])),
      Field(&Turn::stacks, ::testing::IsEmpty()))));
  game->turn();
  game->turn();
}

TEST_F(TwoPlayerGameTests,
       GivenFirstTurnWhenPlayer1PlaysValidCardThenStackOfGivenColorIsCreated)
{
  ON_CALL(*player1, playTurn(::testing::_)).WillByDefault(PlayerPlayCard(0));
  EXPECT_CALL(
    *player2,
    playTurn(AllOf(
      Field(&Turn::numberOfHints, 8),
      Field(&Turn::playerHand,
            ElementsAre(
              deck[1].id, deck[3].id, deck[5].id, deck[7].id, deck[9].id)),
      Field(&Turn::otherPlayers,
            ElementsAre(std::pair<PlayerId, Cards>{
              PLAYER_1_ID,
              Cards{ deck[2], deck[4], deck[6], deck[8], deck[10] } })),
      Field(&Turn::graveyard, ::testing::IsEmpty()),
      Field(&Turn::stacks,
            ElementsAre(std::pair<Color, Value>{ Color::BLUE, Value::ONE })))));
  game->turn();
  game->turn();
}

ACTION_P(PlayerDiscardCard, cardId)
{
  arg0.discard(cardId);
}

TEST_F(
  TwoPlayerGameTests,
  GivenFirstTurnWhenPlayer1DiscardsCardNotInHisHandThenCardNotInHandExceptionIsThrown)
{
  ON_CALL(*player1, playTurn(::testing::_))
    .WillByDefault(PlayerDiscardCard(1000));
  EXPECT_THROW({ game->turn(); }, CardNotInHandException);
}

TEST_F(
  TwoPlayerGameTests,
  GivenFirstTurnWhenPlayer1PlaysDiscardsCardThenAvailableHintsAreNotIncreased)
{
  ON_CALL(*player1, playTurn(::testing::_)).WillByDefault(PlayerDiscardCard(0));
  EXPECT_CALL(
    *player2,
    playTurn(AllOf(
      Field(&Turn::numberOfHints, 8),
      Field(&Turn::playerHand,
            ElementsAre(
              deck[1].id, deck[3].id, deck[5].id, deck[7].id, deck[9].id)),
      Field(&Turn::otherPlayers,
            ElementsAre(std::pair<PlayerId, Cards>{
              PLAYER_1_ID,
              Cards{ deck[2], deck[4], deck[6], deck[8], deck[10] } })),
      Field(&Turn::stacks, ::testing::IsEmpty()),
      Field(&Turn::graveyard, ElementsAre(deck[0])))));
  game->turn();
  game->turn();
}

TEST_F(TwoPlayerGameTests,
       GivenPlayerOneUsedHintWhenPlayerTwoDiscardsCardThenHintIsRestored)
{
  {
    ::testing::InSequence seq;
    EXPECT_CALL(
      *player1,
      playTurn(::testing::AllOf(
        Field(&Turn::otherPlayers,
              ElementsAre(std::pair<PlayerId, Cards>{
                PLAYER_2_ID,
                Cards{ deck[1], deck[3], deck[5], deck[7], deck[9] } })),
        Field(&Turn::graveyard, ::testing::IsEmpty()))))
      .WillOnce(PlayerGiveHint(PLAYER_2_ID, Value::ONE));
    ON_CALL(*player2, playTurn(::testing::_))
      .WillByDefault(PlayerDiscardCard(1));
    EXPECT_CALL(
      *player1,
      playTurn(::testing::AllOf(
        Field(&Turn::numberOfHints, 8),
        Field(&Turn::numberOfLives, 3),
        Field(&Turn::playerHand,
              ElementsAre(
                deck[0].id, deck[2].id, deck[4].id, deck[6].id, deck[8].id)),
        Field(&Turn::otherPlayers,
              ElementsAre(std::pair<PlayerId, Cards>{
                PLAYER_2_ID,
                Cards{ deck[3], deck[5], deck[7], deck[9], deck[10] } })),
        Field(&Turn::graveyard, ElementsAre(deck[1])),
        Field(&Turn::stacks, ::testing::IsEmpty()))));
  }
  game->turn();
  game->turn();
  game->turn();
}

TEST_F(
  TwoPlayerGameTests,
  GivenPlayersUsedUpAllTheHintsWhenPlayerTriesToUseAHintThenNoMoreHintsAvailableExceptionIsThrown)
{
  ON_CALL(*player1, playTurn(::testing::_))
    .WillByDefault(PlayerGiveHint(PLAYER_2_ID, Value::ONE));
  ON_CALL(*player2, playTurn(::testing::_))
    .WillByDefault(PlayerGiveHint(PLAYER_1_ID, Value::ONE));

  for (int i = 0; i < MAX_HINTS; ++i)
    game->turn();

  EXPECT_THROW(game->turn(), NoMoreHintsAvailableException);
}

TEST_F(
  TwoPlayerGameTests,
  GivenPlayersUsedUpAllLivesWhenPlayerTriesToTakeTurnGameIsOverExceptionIsThrown)
{
  EXPECT_CALL(*player1, playTurn(Field(&Turn::numberOfLives, 3)))
    .WillOnce(PlayerPlayCard(2));
  EXPECT_CALL(*player2, playTurn(Field(&Turn::numberOfLives, 2)))
    .WillOnce(PlayerPlayCard(5));
  EXPECT_CALL(*player1, playTurn(Field(&Turn::numberOfLives, 1)))
    .WillOnce(PlayerPlayCard(4));

  for (int i = 0; i < 3; ++i)
    game->turn();

  EXPECT_THROW(game->turn(), GameIsOverException);
}

TEST_F(
  TwoPlayerGameTests,
  GivenPlayers1OpenedColorStackWhenPlayer2PlaysCardOfOtherColorWithNextValueThenLifeIsLost)
{
  EXPECT_CALL(*player1, playTurn(::testing::_)).WillOnce(PlayerPlayCard(0));
  ON_CALL(*player2, playTurn(::testing::_)).WillByDefault(PlayerPlayCard(5));
  EXPECT_CALL(
    *player1,
    playTurn(::testing::AllOf(
      Field(&Turn::numberOfHints, 8),
      Field(&Turn::numberOfLives, 2),
      Field(&Turn::playerHand,
            ElementsAre(
              deck[2].id, deck[4].id, deck[6].id, deck[8].id, deck[10].id)),
      Field(&Turn::otherPlayers,
            ElementsAre(std::pair<PlayerId, Cards>{
              PLAYER_2_ID, Cards{ deck[1], deck[3], deck[7], deck[9] } })),
      Field(&Turn::graveyard, Cards{ deck[5] }),
      Field(&Turn::stacks,
            ElementsAre(std::pair<Color, Value>{ Color::BLUE, Value::ONE })))));

  for (int i = 0; i < 3; ++i)
    game->turn();
}

TEST_F(
  TwoPlayerGameTests,
  GivenPlayers1OpenedColorStackWhenPlayer2PlaysCardOfSameColorWithNextValueThenCardIsAddedToStack)
{
  EXPECT_CALL(*player1, playTurn(::testing::_)).WillOnce(PlayerPlayCard(0));
  ON_CALL(*player2, playTurn(::testing::_)).WillByDefault(PlayerPlayCard(9));
  EXPECT_CALL(
    *player1,
    playTurn(::testing::AllOf(
      Field(&Turn::numberOfHints, 8),
      Field(&Turn::numberOfLives, 3),
      Field(&Turn::playerHand,
            ElementsAre(
              deck[2].id, deck[4].id, deck[6].id, deck[8].id, deck[10].id)),
      Field(&Turn::otherPlayers,
            ElementsAre(std::pair<PlayerId, Cards>{
              PLAYER_2_ID, Cards{ deck[1], deck[3], deck[5], deck[7] } })),
      Field(&Turn::graveyard, Cards{}),
      Field(&Turn::stacks,
            ElementsAre(std::pair<Color, Value>{ Color::BLUE, Value::TWO })))));

  for (int i = 0; i < 3; ++i)
    game->turn();
}

TEST_F(
  TwoPlayerGameTests,
  GivenNoChanceForPerfectScoreWhenPlayerDrawsLastCardFromDeckGameEndsAfterHisNextTurn)
{
  {
    ::testing::InSequence seq;
    EXPECT_CALL(*player1, playTurn(::testing::_))
      .WillOnce(PlayerDiscardCard(4));
    EXPECT_CALL(*player2, playTurn(::testing::_)).WillOnce(PlayerPlayCard(1));
    EXPECT_CALL(*player1, playTurn(::testing::_)).WillOnce(PlayerPlayCard(0));
    EXPECT_CALL(*player2, playTurn(::testing::_))
      .WillRepeatedly(PlayerPlayCard(9));
  }

  for (int i = 0; i < 3; ++i)
    game->turn();

  EXPECT_THROW(game->turn(), GameIsOverException);
}

TEST_F(TwoPlayerGameTests,
       GivenTheGameHasFinishedWithOneRedAndTwoBlueOnTableThenScoreIsThree)
{
  {
    ::testing::InSequence seq;
    EXPECT_CALL(*player1, playTurn(::testing::_))
      .WillOnce(PlayerDiscardCard(4));
    EXPECT_CALL(*player2, playTurn(::testing::_)).WillOnce(PlayerPlayCard(1));
    EXPECT_CALL(*player1, playTurn(::testing::_)).WillOnce(PlayerPlayCard(0));
    EXPECT_CALL(*player2, playTurn(::testing::_))
      .WillRepeatedly(PlayerPlayCard(9));
  }

  for (int i = 0; i < 3; ++i)
    game->turn();

  EXPECT_EQ(game->getScore(), 2);
}

class VerySimplePlayer : public Player
{
  const PlayerId id;

public:
  VerySimplePlayer(const PlayerId& id)
    : id(id)
  {
  }
  PlayerId getId() const override { return id; };
  void playTurn(Turn& turn) override
  {
    if (not turn.playerHand.empty())
      turn.playCard(turn.playerHand.front());
    else
    {
      turn.giveHint(turn.otherPlayers.begin()->first,
                    turn.otherPlayers.begin()->second.front().color);
    }
  };
  void takeHint(PlayerId, std::list<CardId>, Color) override{};
  void takeHint(PlayerId, std::list<CardId>, Value) override{};
};

struct TwoPlayerFullGameTests : public ::testing::Test
{
  TwoPlayerFullGameTests()
    : deck({ { 1, Color::BLUE, Value::ONE },
             { 2, Color::BLUE, Value::TWO },
             { 3, Color::BLUE, Value::THREE },
             { 4, Color::BLUE, Value::FOUR },
             { 5, Color::BLUE, Value::FIVE },
             { 6, Color::RED, Value::ONE },
             { 7, Color::RED, Value::TWO },
             { 8, Color::RED, Value::THREE },
             { 9, Color::RED, Value::FOUR },
             { 10, Color::RED, Value::FIVE },
             { 11, Color::WHITE, Value::ONE },
             { 12, Color::WHITE, Value::TWO },
             { 13, Color::WHITE, Value::THREE },
             { 14, Color::WHITE, Value::FOUR },
             { 15, Color::WHITE, Value::FIVE },
             { 16, Color::YELLOW, Value::ONE },
             { 17, Color::YELLOW, Value::TWO },
             { 18, Color::YELLOW, Value::THREE },
             { 19, Color::YELLOW, Value::FOUR },
             { 20, Color::YELLOW, Value::FIVE },
             { 21, Color::GREEN, Value::ONE },
             { 22, Color::GREEN, Value::TWO },
             { 23, Color::GREEN, Value::THREE },
             { 24, Color::GREEN, Value::FOUR },
             { 25, Color::GREEN, Value::FIVE },
             { 26, Color::GREEN, Value::ONE },
             { 27, Color::GREEN, Value::ONE },
             { 28, Color::BLUE, Value::ONE },
             { 29, Color::BLUE, Value::ONE },
             { 30, Color::WHITE, Value::ONE },
             { 31, Color::WHITE, Value::ONE },
             { 32, Color::RED, Value::ONE } })
    , players{ player1, player2 }
    , game()
  {
    game = std::make_unique<Game>(players,
                                  std::list<Card>(deck.begin(), deck.end()));
  }

  std::vector<Card> deck;
  std::shared_ptr<Player> player1 = std::make_shared<VerySimplePlayer>(1);
  std::shared_ptr<Player> player2 = std::make_shared<VerySimplePlayer>(2);
  Players players;
  std::unique_ptr<Game> game;
};

TEST_F(TwoPlayerFullGameTests,
       WhenGameReachesEndThenGameHasScore25AndGameIsOverIsThrown)
{
  for (auto i = 0; i < 25; ++i)
  {
    game->turn();
  }
  EXPECT_EQ(game->getScore(), 25);
  EXPECT_THROW(game->turn(), GameIsOverException);
}