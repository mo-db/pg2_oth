#include "std_lib_facilities.h"

int main() {
	array<int, 3> a { 8, 3, 9 };
	for (int e : a) {
		e = 5;
	}

	for (int e : a) {
		cout << e << endl;
	}
	
	// if I want to change the array contents -> need to use reference &
	for (int &e : a) {
		e = 5;
	}
	
	for (int e : a) {
		cout << e << endl;
	}
	
	// in all containers are * and ++ operators overloaded
	// iterators are not pointers but are dereferenced with *
	// the container is a struct or class and begin() is a member function
	// *t is a function call
	for (array<int, 3>::iterator t = a.begin();
			t < a.end()-1; t++) {
		cout << *t << " ";
	}
	cout << endl;

	// .begin is a function call that returns an iterator
	// 3. parameter doesnt have to be lambda
	// 3. parameter gets value for each iteration from for_each
	for_each (a.begin(),
			a.end(),
			[](int i) {cout << i << " ";}); // Nameless function as a parameter -> lamda
	cout << endl;

	// proxy iterator, iterator over bool -> && rvalue reference
	// but this is no rvalue reference
	// collection of bool is stored intern as bitfield
	// thus iterating is special
	vector<bool> d(4711);
	for (bool &&e : d) {
		e = false;
	}
	
	return 0;
}
