// CPP lession/syntax collection

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;



void no_category() {
	// terminate from anywhere
	exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);

}

// NOTE [05-05-2025] - Move Semantics
// NOTE [operator overloading] -> function with special name
//
// Restrictions:
// - operator must allready exist
// - at least one operand must be class/enum type
// - must return something
//
// operators can be overloaded as normal, friend or member function

// NOTE friend variant
class FriendCents {
	private:
		int cents {};
	public:
		// constructor
		FriendCents(int cents) : cents{ cents } {}
		// in class definition
		friend int operator +(const FriendCents &c1, const FriendCents &c2) {
			return c1.cents + c2.cents;
		}
		// in class decleration only
		friend int operator -(const FriendCents &c1, const FriendCents &c2);
};

// out of class definition
int operator-(const FriendCents &c1, const FriendCents &c2) {
	return c1.cents - c2.cents;
}

// NOTE member variant
class MemberCents{
	private:
		int cents {};
	public:
		// constructor
		MemberCents(int cents) { this->cents = cents; }

		// first const for parameter, second for this operand
		int operator +(const MemberCents &c) const{
			return this->cents + c.cents; // TODO why pointer deref here?
		}

		// member funcitons can use this->
		MemberCents operator -(const MemberCents &c) const;
};

MemberCents MemberCents::operator -(const MemberCents &c) const {
	return this->cents - c.cents;
}

void operator_overloading() {
	FriendCents fc1 {7};
	FriendCents fc2{2};
	int sum = 4 - fc1;
	sum = fc1 - 4;
	cout << "cents sum: " << sum << endl;
	sum = fc1 - fc2;
	cout << "cents sum: " << sum << endl;

	MemberCents mc1 {2};
	MemberCents mc2 {7};
	sum = mc1 + 4;
	// sum = 4 + mc1; -> ERROR
	cout << "cents sum: " << sum << endl;
}

void move_semantics() {
	class Foo {

		// operator +(int, int);
	};
}

// NOTE dynamic memory, stack and heap
void dynamic_memory() {
	// NOTE STL
	// object and data on heap -> destroyed on program end or delete keyword
	vector<double> *vp = new vector<double> { 1.1, 2.2 };
	// object on stack, data on heap -> destroyed on scope exit
	vector<double> v { 3.3, 4.4 };

	// NOTE prefer using RAII containers
	
	// NOTE testing
	cout << v.at(1) << endl;
	cout << (*vp).at(1) << endl;
	delete vp;
};

void constructors() {
	class Obj {
		int var {};
		public:
		// Obj (int var) : var{ var } {};
	};
	// default conctructor use
	Obj o1 {};
  // default copy constructor use
	Obj o2 { o1 };
	// move constructor use -> destroys o1 -> UB when using o1 afterwards
	// returns rvalue reference -> &&
	Obj o3 = std::move(o1);
	
	class Obj2 {
		int var {};
		public:
		Obj2 (const int var) : var{ var } {};
		// copy constructor
		Obj2 (const Obj2 &o) { var = o.var; }
		// move constructor -> move semantics in RUST is ownership
		Obj2 (const Obj2 &&o) { var = o.var; }; // efficient
	};
}

void lvalue_rvalue() {
	// LValues are constant
	// PRValues (pure RValues) -> temporary objects or no objects at all
	// XValues (eXpiring) -> objects that can be moved

	int &foo(); // foo() -> LValue
	int bar(); // bar() -> PRValue
	int &&foo_bar(); // foo_bar() -> XValue

	int a, b;
	int c = a * b; // works
	//int a * b = c; -> a * b operator returns rvalue, cant stand on the left
	int *p = &a; // works
	//p = &43; -> rvalue has no address
}


int main(int argc, char **argv) {
	// overload_test();
	dynamic_memory();
	return 0;
}
