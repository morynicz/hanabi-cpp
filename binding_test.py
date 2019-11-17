import unittest
import hanabi_py as h


class PyPlayer(h.Player):
    def __init__(self, id):
        h.Player.__init__(self)
        self.id = id

    def getId(self):
        return self.id

    def playTurn(self, turn):
        turn.play_card(turn.player_hand[0])

    def takeHint(self, ids, arg):
        pass


def make_card(id, color, value):
    c = h.Card()
    c.id = id
    c.color = color
    c.value = value
    return c


class MyTestCase(unittest.TestCase):
    def test_simple_game(self):
        p1 = PyPlayer(1)
        p2 = PyPlayer(2)
        cards = [
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
        g = h.Game([p1, p2], cards)
        for _ in range(25):
            g.turn()
        self.assertEqual(25, g.getScore())


if __name__ == '__main__':
    unittest.main()
