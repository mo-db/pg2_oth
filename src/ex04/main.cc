#include "std_lib_facilities.h"

// typesave scoped enumerator
enum struct Color {
	kreuz,
	pik,
	karo,
	herz,
};

// typesave scoped enumerator
enum struct Name {
	sieben,
	acht,
	neun,
	bube,
	dame,
	koenig,
	zehn,
	as,
};

struct Card {
	Name name;
	Color color;
};

// print out a card, scoped enums cannot just be printed as string
void print_card(Card *card) {
	switch (card->color) {
		case Color::kreuz: cout << "Kreuz, "; break;
		case Color::karo: cout << "Karo, "; break;
		case Color::pik: cout << "Pik, "; break;
		case Color::herz: cout << "Herz, "; break;
	}
	switch (card->name) {
		case Name::sieben: cout << "sieben" << endl; break;
		case Name::acht: cout << "acht" << endl; break;
		case Name::neun: cout << "neun" << endl; break;
		case Name::bube: cout << "bube" << endl; break;
		case Name::dame: cout << "dame" << endl; break;
		case Name::koenig: cout << "koenig" << endl; break;
		case Name::zehn: cout << "zehn" << endl; break;
		case Name::as: cout << "as" << endl; break;
	}
}

// initialize an array with all possible cards, and then mix it
// using the random number generator
void skat_init(Card *skat) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			Card *card = skat + i * 8 + j;
			switch (i) {
				case 0: card->color = Color::kreuz; break;
				case 1: card->color = Color::pik; break;
				case 2: card->color = Color::karo; break;
				case 3: card->color = Color::herz; break;
			}
			switch (j) {
				case 0: card->name = Name::sieben ; break;
				case 1: card->name = Name::acht ; break;
				case 2: card->name = Name::neun ; break;
				case 3: card->name = Name::bube ; break;
				case 4: card->name = Name::dame ; break;
				case 5: card->name = Name::koenig ; break;
				case 6: card->name = Name::zehn ; break;
				case 7: card->name = Name::as; break;
			}
		}
	}
	// mix the array
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> dis(0, 31);
	for (int i = 0; i < 32; i++) {
		int position = dis(gen);
		Card tmp = skat[i];
		skat[i] =	*(skat + position);
		skat[position] = tmp;
	}
}

int main() {
	// can use cstyle or c++ array
	array<Card, 32> skat_array {};
	// Card skat_array[32];
	skat_init(skat_array.data()); // c++ container muss .data() verwendet werden

	// print the last two cards that remain with 3 players
	print_card(&skat_array[30]);
	print_card(&skat_array[31]);

	// for (int i = 0; i < 32; i++) {
	// 	print_card(&skat_array[i]);
	// }
  return 0;
}
