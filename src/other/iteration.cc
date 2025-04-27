#include <vector>
#include <iostream>
#include <algorithm>
void print_data(int x) {
	std::cout << "value: " << x << std::endl;
}

int main() {

	std::array<int, 5> data { 1, 2, 3, 4, 5 };

	for (std::array<int, 5>::iterator traverse = data.begin();
			 traverse != data.end(); ++traverse) {
		*traverse *= 100;
	}

	for (auto traverse = data.begin();
			 traverse != data.end(); ++traverse) {
		*traverse *= 2;
	}
	
	std::for_each(data.begin(), data.end(), [](int& x) {
			x--;
			});

	std::ranges::for_each(data, [](int& x) {
			x--;
			});

	std::ranges::for_each(data, print_data);

	// structured binding -> iteration over touple (std::map)
	// for (auto& [word, count] : occurences) {;}

	for (auto value : data) {
		std::cout << value << std::endl;
	}
  return 0;
}
