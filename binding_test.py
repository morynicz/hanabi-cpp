import unittest
import hanabi_py as h


class FirstCardInHandPlayingPlayer(h.Player):
    def __init__(self, id):
        h.Player.__init__(self)
        self.id = id

    def getId(self):
        return self.id

    def playTurn(self, turn):
        turn.playCard(turn.playerHand[0])

    def takeHint(self, playerId, ids, arg):
        pass


def make_card(id, color, value):
    c = h.Card()
    c.id = id
    c.color = color
    c.value = value
    return c


class MyTestCase(unittest.TestCase):
    def setUp(self) -> None:
        self.cards = [
            make_card(1, h.Color.BLUE, h.Value.ONE),
            make_card(2, h.Color.BLUE, h.Value.TWO),
            make_card(3, h.Color.BLUE, h.Value.THREE),
            make_card(4, h.Color.BLUE, h.Value.FOUR),
            make_card(5, h.Color.BLUE, h.Value.FIVE),
            make_card(6, h.Color.RED, h.Value.ONE),
            make_card(7, h.Color.RED, h.Value.TWO),
            make_card(8, h.Color.RED, h.Value.THREE),
            make_card(9, h.Color.RED, h.Value.FOUR),
            make_card(10, h.Color.RED, h.Value.FIVE),
            make_card(11, h.Color.WHITE, h.Value.ONE),
            make_card(12, h.Color.WHITE, h.Value.TWO),
            make_card(13, h.Color.WHITE, h.Value.THREE),
            make_card(14, h.Color.WHITE, h.Value.FOUR),
            make_card(15, h.Color.WHITE, h.Value.FIVE),
            make_card(16, h.Color.YELLOW, h.Value.ONE),
            make_card(17, h.Color.YELLOW, h.Value.TWO),
            make_card(18, h.Color.YELLOW, h.Value.THREE),
            make_card(19, h.Color.YELLOW, h.Value.FOUR),
            make_card(20, h.Color.YELLOW, h.Value.FIVE),
            make_card(21, h.Color.GREEN, h.Value.ONE),
            make_card(22, h.Color.GREEN, h.Value.TWO),
            make_card(23, h.Color.GREEN, h.Value.THREE),
            make_card(24, h.Color.GREEN, h.Value.FOUR),
            make_card(25, h.Color.GREEN, h.Value.FIVE),
            make_card(26, h.Color.GREEN, h.Value.ONE),
            make_card(27, h.Color.GREEN, h.Value.ONE),
            make_card(28, h.Color.BLUE, h.Value.ONE),
            make_card(29, h.Color.BLUE, h.Value.ONE),
            make_card(30, h.Color.WHITE, h.Value.ONE),
            make_card(31, h.Color.WHITE, h.Value.ONE),
            make_card(32, h.Color.RED, h.Value.ONE)
        ]

    def test_simple_game(self):
        p1 = FirstCardInHandPlayingPlayer(1)
        p2 = FirstCardInHandPlayingPlayer(2)

        g = h.Game([p1, p2], self.cards)
        for _ in range(25):
            g.turn()
        self.assertEqual(25, g.getScore())

PLAYER_1_ID = 1
PLAYER_2_ID = 2

class Hinter(h.Player):
    def __init__(self, giveHint):
        self.hintTaken = False
        h.Player.__init__(self)
        self.id = PLAYER_1_ID
        self.giveHint = giveHint

    def getId(self):
        return self.id

    def playTurn(self, turn):
        turn.giveHint(PLAYER_2_ID, self.giveHint(turn.otherPlayers[PLAYER_2_ID][0]))

    def takeHint(self, playerId, ids, arg):
        pass

class HintTaker(h.Player):
    def __init__(self):
        h.Player.__init__(self)
        self.id = PLAYER_2_ID

    def getId(self):
        return self.id

    def playTurn(self, turn):
        self.hints = turn.numberOfHints

    def takeHint(self, playerId, ids, arg):
        self.ids = ids
        self.arg = arg
        self.playerId = playerId


class HintTakingTests(MyTestCase):
    def setUp(self) -> None:
        super(HintTakingTests, self).setUp()

    def test_color_hint_from_p1_is_sent_to_p2(self):
        def giveHint(card):
            return card.color
        p1 = Hinter(giveHint)
        p2 = HintTaker()
        blue_card_ids = [2, 4]

        g = h.Game([p1, p2], self.cards)
        g.turn()
        self.assertEqual(p2.ids, blue_card_ids)
        self.assertEqual(p2.arg, h.Color.BLUE)
        self.assertEqual(p2.playerId, PLAYER_2_ID)

    def test_value_hint_from_p1_is_sent_to_p2(self):
        def giveHint(card):
            return card.value
        p1 = Hinter(giveHint)
        p2 = HintTaker()

        g = h.Game([p1, p2], self.cards)
        g.turn()
        value_2_card_ids = [2]
        self.assertEqual(p2.ids, value_2_card_ids)
        self.assertEqual(p2.arg, h.Value.TWO)
        self.assertEqual(p2.playerId, PLAYER_2_ID)


    def test_after_using_hint_p2_gets_turn_with_7_hints(self):
        def giveHint(card):
            return card.value
        p1 = Hinter(giveHint)
        p2 = HintTaker()

        g = h.Game([p1, p2], self.cards)
        g.turn()
        g.turn()
        self.assertEqual(p2.hints, 7)

if __name__ == '__main__':
    unittest.main()
