#include "Game.hpp"
#include "pybind11/pybind11.h"
namespace py = pybind11;

// Game makeGame(const Players& players, const Cards& cards)
//{
//    return Game(players, cards);
//}

class PyPlayer : public Player
{
  using Player::Player;

  PlayerId getId() const override
  {
    PYBIND11_OVERLOAD_PURE(PlayerId, Player, getId, );
  }

  void playTurn(Turn& turn) override
  {
    PYBIND11_OVERLOAD_PURE(void, Player, playTurn, turn);
  }

  void takeHint(std::list<CardId> ids, Color color) override
  {
    PYBIND11_OVERLOAD_PURE(void, Player, playTurn, ids, color);
  }

  void takeHint(std::list<CardId> ids, Value value) override
  {
    PYBIND11_OVERLOAD_PURE(void, Player, playTurn, ids, value);
  }
};

PYBIND11_MODULE(hanabi_py, m)
{
  m.doc() = "hanabi-cpp wrapper for python";
  //    m.def("makeGame", &makeGame, "Game factory", py::arg("players"),
  //    py::arg("cards"));
  py::class_<Game>(m, "Game")
    .def(py::init<const Players&, const Cards&>())
    .def("turn", &Game::turn)
    .def("getScore", &Game::getScore);

  py::class_<Player, PyPlayer>(m, "Player")
    .def(py::init<>())
    .def("getId", &Player::getId, "Get id")
    .def("playTurn", &Player::playTurn, "Play turn", py::arg("turn"))
    .def("takeHint",
         py::overload_cast<std::list<CardId>, Color>(&Player::takeHint),
         "take color hint")
    .def("takeHint",
         py::overload_cast<std::list<CardId>, Value>(&Player::takeHint),
         "take value hint");
}
