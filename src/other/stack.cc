#include <iostream>
#include <chrono>
#include <limits>
#include <cstring>

using namespace std;

// use constructor and destructor if default concrete enough
struct Stack {
	int *bottom;
	int *top;
	int limit;
	Stack(int l) : bottom {new int [l]}, top {bottom}, limit {l} {}
	~Stack() { delete [] bottom; }
	void copy(const Stack &);
	// void copy(Stack &) const; // this function cannot change anything
	void fill(int);
};

void Stack::copy(const Stack &copy_stack) {
	if (this != &copy_stack) { // this is of type *, so need address of stack ref
		delete [] bottom;
		limit = copy_stack.limit;
		bottom = top = new int [limit];
		// memcpy 15x faster
		std::memcpy(bottom, copy_stack.bottom, limit * sizeof(int));
		// for (int i = 0; i < limit; i++) {
		// 	*top++ = (*copy_stack.bottom)++;
		// }
	}
}

void Stack::fill(int value) {
	for (int i = 0; i < limit; i++) {
		*top++ = value;
	}
}

// ERROR: double free bottom?
// should work for stack + number, number + stack, stack + stack
struct Stack2 {
	int *bottom, *top, limit;
	Stack2(int l) : bottom {new int [l]}, top {bottom}, limit {l} {}
	~Stack2() { delete [] bottom; }
	int peek(int p) const {return *(bottom + p); }
	void fill(int);
	Stack2 operator +(const Stack2 a) {
		Stack2 r = Stack2(limit + a.limit);
		for (int i = 0; i < limit; i++)
			r.push(peek(i));
		for (int i = 0; i < a.limit; i++)
			r.push(a.peek(i));
		return r;
	}
	void push(int i) {
		if ((top - bottom) < limit)
			*top++ = i;
	}
};

void Stack2::fill(int value) {
	for (int i = 0; i < limit; i++) {
		*top++ = value;
	}
}

int main() {
	int value = numeric_limits<int>::max();
	Stack s1 {value/100};
	Stack s2 {value/100};
	s1.fill(3);
	s2.fill(7);
	s1.copy(s2);

	cout << "stack 1 bottom: " << *(s1.bottom) << endl;
	cout << "stack 1 size: " << s1.limit << endl;

	Stack2 s3 {value/500};
	Stack2 s4 {value/300};
	s3.fill(4);
	s4.fill(5);

	auto t1 = chrono::steady_clock::now();
	Stack2 s5 = s3 + s4;
	auto t2 = chrono::steady_clock::now();
	chrono::duration<double, std::milli> duration = t2 - t1;
	cout << "copy duration: " << duration << endl;
	cout << "stack 3 size: " << s3.limit << endl;
	cout << "stack 4 size: " << s4.limit << endl;
	cout << "stack 5 size: " << s5.limit << endl;

	return 0;
}
