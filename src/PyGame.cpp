#include "Game.hpp"
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
    .def(py::init<const Players&, const Cards&>())
    .def("turn", &Game::turn)
    .def("getScore", &Game::getScore);

  py::class_<Player, std::shared_ptr<Player>, PyPlayer>(m, "Player")
    .def(py::init<>())
    .def("getId", &Player::getId, "Get id")
    .def("playTurn", &Player::playTurn, "Play turn", py::arg("turn"))
    .def("takeHint",
         py::overload_cast<std::list<CardId>, Color>(&Player::takeHint),
         "take color hint")
    .def("takeHint",
         py::overload_cast<std::list<CardId>, Value>(&Player::takeHint),
         "take value hint");

  py::class_<Card>(m, "Card")
    .def_readwrite("color", &Card::color)
    .def_readwrite("value", &Card::value)
    .def_readwrite("id", &Card::id);

  py::enum_<Value>(m, "Value")
    .value("Unknown", Value::UNKNOWN)
    .value("One", Value::ONE)
    .value("Two", Value::TWO)
    .value("Three", Value::THREE)
    .value("Four", Value::FOUR)
    .value("Five", Value::FIVE)
    .export_values();

  py::enum_<Color>(m, "Color")
    .value("Red", Color::RED)
    .value("Green", Color::GREEN)
    .value("Blue", Color::BLUE)
    .value("Yellow", Color::YELLOW)
    .value("White", Color::WHITE)
    .value("Rainbow", Color::RAINBOW)
    .export_values();
}
