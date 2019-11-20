#include "Game.hpp"
#include "pybind11/functional.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include <sstream>
namespace py = pybind11;

std::ostream& operator<<(std::ostream& str, Color color)
{
  switch (color)
  {
    case Color::UNKNOWN:
      return str << "UNKNOWN";
    case Color::WHITE:
      return str << "WHITE";
    case Color::GREEN:
      return str << "GREEN";
    case Color::BLUE:
      return str << "BLUE";
    case Color::RED:
      return str << "RED";
    case Color::YELLOW:
      return str << "YELLOW";
    case Color::RAINBOW:
      return str << "RAINBOW";
    default:
      return str << "UNKNOWN";
  }
}

std::ostream& operator<<(std::ostream& str, Value value)
{
  switch (value)
  {
    case Value::ONE:
      return str << "ONE";
    case Value::TWO:
      return str << "TWO";
    case Value::THREE:
      return str << "THREE";
    case Value::FOUR:
      return str << "FOUR";
    case Value::FIVE:
      return str << "FIVE";
    default:
      return str << "UNKNOWN";
  }
}

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

  void takeHint(PlayerId playerId, std::list<CardId> ids, Color color) override
  {
    PYBIND11_OVERLOAD_PURE(void, Player, takeHint, playerId, ids, color);
  }

  void takeHint(PlayerId playerId, std::list<CardId> ids, Value value) override
  {
    PYBIND11_OVERLOAD_PURE(void, Player, takeHint, playerId, ids, value);
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
    .def(
      "takeHint",
      py::overload_cast<PlayerId, std::list<CardId>, Color>(&Player::takeHint),
      "player got color hint",
      py::arg("playerId"),
      py::arg("cards"),
      py::arg("color"))
    .def(
      "takeHint",
      py::overload_cast<PlayerId, std::list<CardId>, Value>(&Player::takeHint),
      "player got value hint",
      py::arg("playerId"),
      py::arg("cards"),
      py::arg("value"));

  py::class_<Card>(m, "Card")
    .def(py::init<>())
    .def_readwrite("color", &Card::color)
    .def_readwrite("value", &Card::value)
    .def_readwrite("id", &Card::id)
    .def("__str__", [](const Card& card) -> std::string {
      std::stringstream ss;
      ss << "<Card id: " << card.id << " color: " << card.color
         << " value: " << card.value << ">";
      return ss.str();
    });

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
    .def("discard", &Turn::discard, py::arg("cardId"))
    .def_readonly("playerHand", &Turn::playerHand)
    .def_readonly("otherPlayers", &Turn::otherPlayers)
    .def_readonly("graveyard", &Turn::graveyard)
    .def_readonly("stacks", &Turn::stacks)
    .def_readonly("numberOfHints", &Turn::numberOfHints)
    .def_readonly("numberOfLives", &Turn::numberOfLives);
}
