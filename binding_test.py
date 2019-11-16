import unittest
import hanabi_py as h

class PyPlayer(h.Player):
    def __init__(self, id):
        h.Player.__init__(self)
        self.id = id

    def getId(self):
        return self.id

    def playTurn(self,turn):
        pass

    def takeHint(self, ids, arg):
        pass


class MyTestCase(unittest.TestCase):
    def test_something(self):
        p = PyPlayer(1)
        c = h.Card
        c.id = 0
        c.color = h.Color.Red
        c.value = h.Value.One
        g = h.Game([p],[c])
        self.assertEqual(p.getId(), 1)

def attempt():
        p = PyPlayer(1)
        c = h.Card
        c.id = 0
        c.color = h.Color.Red
        c.value = h.Value.One
        g = h.Game([p],[c])

if __name__ == '__main__':
    attempt()
    unittest.main()
