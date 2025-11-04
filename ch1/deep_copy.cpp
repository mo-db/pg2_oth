#include <iostream>

class MyString {
public:
  MyString(const char* in_string);
	MyString(const MyString& source);
  ~MyString();
	MyString operator=(const MyString& source);
	void print();

private:
	void deep_copy(const MyString& source);
	char* str{};
	int length{};
};


MyString::MyString(const char* in_string) {
	std::cout << "constructor called" << std::endl;
	if (!in_string) {
		return;
	}

	// plus one for the termination character
	length = std::strlen(in_string) + 1;

	str = new char[length];

	for (int i = 0; i < length; i++) {
		str[i] = in_string[i];
	}
}


MyString::MyString(const MyString& source) {
	std::cout << "copy-constructor called" << std::endl;
	deep_copy(source);
}


MyString MyString::operator=(const MyString& source) {
	std::cout << "assignment overload called" << std::endl;
	// check for self-assignment
	if (this != &source) {
		deep_copy(source);
	}
	
	return *this;

}


MyString::~MyString() {
	std::cout << "destructor called" << std::endl;
	if (str) { 
		delete str;
	}
}


void MyString::deep_copy(const MyString& source) {
	length = source.length;

	if (!source.str) {
		str = nullptr;
		return;
	}

	str = new char[length];

	for (int i = 0; i < length; i++) {
		str[i] = source.str[i];
	}
}


void MyString::print() {
	if (!str) {
		return;
	};

	std::cout << str << std::endl;
}


int main() {
	MyString my1 {"foo"};
	MyString my2 {my1};
	MyString my3 {"bar"};
	my1 = my3;
	my1.print();
	my2.print();

	return 0;
}
