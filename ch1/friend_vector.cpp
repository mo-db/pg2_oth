#include <iostream>

class Vector2D {
public:
	Vector2D(const double x_, const double y_) : x{x_}, y{y_} {}
	friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
	void print();

private:
	double x{};
	double y{};
};


Vector2D operator+(const Vector2D& v1, const Vector2D& v2) {
	return {v1.x + v2.x, v2.y + v2.y};
}

void Vector2D::print() {
	std::cout << "(" << x << ", " << y << ")" << std::endl;
}

int main() {
	Vector2D v1 {2, 4};
	Vector2D v2 {10, 20};
	Vector2D v3 = v1 + v2;
	v3.print();
	return 0;
}
