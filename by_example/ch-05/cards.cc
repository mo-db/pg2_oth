#include "cards.h"

namespace cards {
std::ostream &operator<<(std::ostream &os, const Card &card) {
  os << to_string(card.value()) << " of " << to_string(card.suit());
  return os;
}

std::string to_string(const Suit & suit)
{
  using namespace std::literals;
  switch (suit) {
  case Suit::Hearts:
		// the s prefix converts the char *literal into a c++ string
    return "Hearts"s;
  case Suit::Diamonds:
    return "Diamonds"s;
  case Suit::Clubs:
    return "Clubs"s;
  case Suit::Spades:
    return "Spades"s;
  default:
    return "?"s;
  }
}

std::string to_string(const FaceValue &value) {
  using namespace std::literals;
  switch (value.value()) {
  case 11:
    return "Jack"s;
  case 12:
    return "Queen"s;
  case 13:
    return "King"s;
  case 14:
    return "Ace"s;
  default:
    return std::to_string(value.value());
  }
}

// NOTE create deck using range-based loop over initializer list
std::array<Card, 52> create_deck_range() {
  std::array<Card, 52> deck;
	// card is type iterator of deck begin
  auto card = deck.begin();
	// range-based for loop over initializer list
	// used this way because there is no general ++ operator for enums
  for (auto suit : {Suit::Hearts, Suit::Diamonds, Suit::Clubs, Suit::Spades}) {
    for (int value = 2; value <= 14; value++) {
			// set card values at deref. of iterator
      *card = Card{FaceValue(value), suit};
			// increment card iterator
      ++card;
    }
  }
  return deck;
}

// NOTE create deck using std::algorithms
// overloading ++ to implement enum version, using enum underlying type
Suit &operator++(Suit &suit) {
	// std::underlying_type<Suit> returns a trait class template
	// appending ::type gets the type 
	// typename is needed in template contexts to tell the compiler, that 
	//   what follows is a type	
	// using keyword is a type alias -> like typedef but cleaner syntax
  using IntType = typename std::underlying_type<Suit>::type;
	// return to first element after last
	if(suit == Suit::Spades) { 
		suit = Suit::Hearts;
	} else {
		// convert to underlying type -> arithmetic -> convert result to enum type
		suit = static_cast<Suit>(static_cast<IntType>(suit) + 1);
	}
	// not needed most of the time
	return suit;
}

std::array<Card, 52> create_deck_algorithm()
{
    std::array<Card, 52> deck;
    int value = 2;
    Suit suit = Suit::Hearts;
    std::ranges::generate(deck, [&value, &suit]() {
        if (value > 14)
        {
           value = 2;
            ++suit;
        }
        return Card{FaceValue(value++), suit};
    });
    return deck;
}

void shuffle_deck(std::array<Card, 52> & deck)
{
    std::random_device rd;
    std::mt19937 gen{ rd() };
    std::ranges::shuffle(deck, gen);
}
} // namespace cards

