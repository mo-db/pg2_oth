#pragma once

#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <compare>
#include <algorithm>
#include <random>

namespace cards {
// typesave enum
// scoped enums have an underlying type -> int by default
enum class Suit : short {
  Hearts,
  Diamonds,
  Clubs,
  Spades,
};

class FaceValue {
public:
	// NOTE explicit because ...
  explicit FaceValue(int value) : value_(value) {
    if (value_ < 2 || value_ > 14) {
      throw std::invalid_argument("Face value invalid");
    }
  }
  int value() const { return value_; }
	// makes FaceValue comparable
  auto operator<=>(const FaceValue&) const = default;
private:
  int value_;
};

// private variables are default initialized for explicit default constructor
class Card {
public:
	Card() = default;
	Card(FaceValue value, Suit suit) : value_{value}, suit_{suit} {}
	FaceValue value() const { return value_; }
	Suit suit() const { return suit_; }
	// implements all comparisons for all data fields, in defined order
	// thie spaceship operator <=> can be defaulted to automatically generate all
	//   comparison operators. Returns a type that indicates the ordering.
	// for a <=> b, if result < 0 then a < b, if result > 0 then a > b, = means =
	auto operator<=>(const Card &) const = default;
private:
	FaceValue value_{2};
	Suit suit_{};
};

// for printing out a card
std::ostream& operator<<(std::ostream & os, const Card & card);
std::string to_string(const Suit & suit);
std::string to_string(const FaceValue &value);

// two different ways to create the deck
std::array<Card, 52> create_deck_range();
std::array<Card, 52> create_deck_algorithm();

// shuffle the cards using std::ranges::shuffle()
void shuffle_deck(std::array<Card, 52> & deck);
} // namespace cards
