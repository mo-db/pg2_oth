// CPP lession/syntax collection

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;



namespace no_category {
	// constexpr >> const, constexpr marks expressions for compile-time eval
	constexpr int cexpr = 32;
	constexpr int a = 6;
	constexpr int b = 7;
	constexpr int meaning_of_life(int a, int b) { return a * b; }

	void no_category() {
		// terminate from anywhere
		exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
		// sizeof returns size in bytes
		sizeof(int); // -> compile-time

		// alignof returns alignment requirement in bytes
		// returns 1 if there are no align requirements for a type
		alignof(double); // -> compile-time
	}
}

// NOTE [05-05-2025] - Move Semantics
// NOTE [operator overloading] -> function with special name
//
// Restrictions:
// - operator must allready exist
// - at least one operand must be class/enum type
// - must return something
// - most operators with . or : dots cant be overloaded
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
		//	return this->cents + c.cents; // TODO why pointer deref here?
		}

		// member funcitons can use this->
		MemberCents operator -(const MemberCents &c) const;
};

MemberCents MemberCents::operator -(const MemberCents &c) const {
	return this->cents - c.cents;
}

// one can overload the call operator to use a value as function?
struct LessThan { // -> functor class, stateless (no attributes)
	bool operator()(double a, double b) { return a < b; }
};

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

	LessThan l;
	int value = l(5, 7);
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

// NOTE 12.05.2025
void constructor_destructor() {
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
		// Default constructor, the first is to be prefered
		Obj2() = default;
		// Obj2() {};

		// some constructor
		Obj2 (const int var) : var{ var } {};
		// copy constructor
		Obj2 (const Obj2 &o) { var = o.var; }
		// move constructor -> move semantics in RUST is ownership
		Obj2 (const Obj2 &&o) { var = o.var; }; // efficient
	};

	// every class has the following default members
	class A {
		A();												// default constructor
		A(const A &);								// copy constructor
		A(A &&);										// move constructor
		~A();												// destructor
		A &operator =(const A &);		// copy assignment operator
		A &operator =(A &&);				// move assignment operator
	};

	// initializer list example
	class First {
		int v;
		public:
		First(int v) { set_v(v); }
		void set_v(int v) { this->v = v; }
	};

	class Second {
		First f; // declares f as type First
		public:
		//Second(int v) { f.set_v(v); } // cant do -> f not initialized
		// using an initializer list: -> this is to be prefared allways
		Second(int v) : f{v} {} // here i use the custom constructor
	};

	// deligate constructor
	class Widget {
		public:
		Widget(char c, int i) : c{c}, i{i} {}
		Widget(int i) : Widget('a', i) {} // this delegate uses custom constructor
		private:
		char c;
		int i;
	};
}

// NOTE class attributes -> variables shared by all instancences of a class
// not allowed in local classes -> only global scope
// also the same thing works for functions -> can be used bevore a class
// is even constructed
class KlassenAttribute {
	static int shared_var;
	static bool is_true();
};

// NOTE mutable variables can be canged in const functions
class Mutable {
	mutable int mut;
	int mutate() const { return mut++; }
};


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

// typename == class
// parameters inside <> must be compile-time
// compiler searches for normal functions first, then for templates
template <typename T> T my_max(T x, T y) { return x > y ? x : y; }
void template_functions() {
	int i, j;
	int i_result;
	double x, y;
	double d_result;
	i_result = my_max(i, j);
	d_result = my_max(x, y);
	// d_result = my_max(x, j); -> error
	d_result = my_max<double>(x, j);
}

// NOTE operator->() overloading has special functionality
struct Content { int data; };
struct Wrapper {
	Content *internal;
	Content *operator->() const {
		return internal;
	}
};

struct Accessor {
	Wrapper *object;
	Wrapper &operator->() const {
		return *object;
	}
};

void template_test() {
	Content a {42};
	Wrapper b { &a };
	Accessor c { &b };
	cout << a.data << b->data << c->data << endl;
	pair<int, int>;
}



int main(int argc, char **argv) {
	// overload_test();
	// dynamic_memory();
	template_test();
	return 0;
}
