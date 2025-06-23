#include "cards.h"

// using namespace cards;
using namespace std;

int main() {
	auto deck = cards::create_deck_algorithm();
	cards::shuffle_deck(deck);
	for (auto card : deck) {
		cout << card << endl;
	}
	// cards::Card card {cards::FaceValue{1}, cards::Suit::Hearts};
	// cout << card << endl;
	return 0;
}
