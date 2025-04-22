#include "std_lib_facilities.h"

struct s {
	int a, b;
	auto operator <=>(const s &s2) const {
		cout << "bla" << endl;
		cout << (a - s2.a) << endl;
		return a - s2.a;
	}
};

int main() {
	s da { 5, 7 }, db { 6, 8 };
	int s::*ip = &s::b;
	cout << db.*ip << endl;
	if (da > db) {
		cout << "heureka" << endl;
	}
	if (da.*ip > db.*ip) {
		cout << "heureka" << endl;
	}
	return 0;
}
