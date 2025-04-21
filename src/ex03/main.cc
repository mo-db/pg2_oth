#include <iostream>

using namespace std;

std::array<int, 4> g_values { 1, 2, 3, 4, };
int test_func() {
	std::array<int, 4> values { 4, 3, 2, 1, };
	return values[1];
}

struct valueHolder {
	int value;
	void print() {
		std::cout << value << std::endl;
	};
};

int main() {
	valueHolder data;
	data.value = 4711;
	data.print();
  return 0;
}
