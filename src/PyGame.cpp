#include "Game.hpp"
#include "pybind11/functional.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
namespace py = pybind11;

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
  py::class_<Game>(m, "Game")
    .def(py::init<const Players&, const Cards&>(),
         py::arg("players"),
         py::arg("cards"))
    .def("turn", &Game::turn)
    .def("getScore", &Game::getScore);

  py::class_<Player, std::shared_ptr<Player>, PyPlayer>(m, "Player")
    .def(py::init<>())
    .def("getId", &Player::getId, "Get id")
    .def("playTurn", &Player::playTurn, "Play turn", py::arg("turn"))
    .def("takeHint",
         py::overload_cast<std::list<CardId>, Color>(&Player::takeHint),
         "take color hint",
         py::arg("cards"),
         py::arg("color"))
    .def("takeHint",
         py::overload_cast<std::list<CardId>, Value>(&Player::takeHint),
         "take value hint",
         py::arg("cards"),
         py::arg("value"));

  py::class_<Card>(m, "Card")
    .def(py::init<>())
    .def_readwrite("color", &Card::color)
    .def_readwrite("value", &Card::value)
    .def_readwrite("id", &Card::id);

  py::enum_<Value>(m, "Value")
    .value("UNKNOWN", Value::UNKNOWN)
    .value("ONE", Value::ONE)
    .value("TWO", Value::TWO)
    .value("THREE", Value::THREE)
    .value("FOUR", Value::FOUR)
    .value("FIVE", Value::FIVE)
    .export_values();

  py::enum_<Color>(m, "Color")
    .value("RED", Color::RED)
    .value("GREEN", Color::GREEN)
    .value("BLUE", Color::BLUE)
    .value("YELLOW", Color::YELLOW)
    .value("WHITE", Color::WHITE)
    .value("RAINBOW", Color::RAINBOW)
    .export_values();

  py::class_<Turn>(m, "Turn")
    .def("giveHint",
         py::overload_cast<PlayerId, Color>(&Turn::giveHint),
         "give color hint",
         py::arg("playerId"),
         py::arg("Color"))
    .def("giveHint",
         py::overload_cast<PlayerId, Value>(&Turn::giveHint),
         "give value hint",
         py::arg("playerId"),
         py::arg("Value"))
    .def("playCard", &Turn::playCard, py::arg("cardId"))
    .def("discard", &Turn::discard, py::arg("cardId"));
}
