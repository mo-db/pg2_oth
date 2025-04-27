#pragma once

namespace cards {
	enum class Suit { // typesave enum
		Hearts,
		Diamonds,
		Clubs,
		Spades,
	};
	struct Card {
		int value{}; // {} for default initialization
		Suit suit{};
	};
}
