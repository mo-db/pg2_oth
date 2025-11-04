#include "core.hpp"

namespace core {
bool equal_epsilon(double x, double y) {
	return (x < y + epsilon && x > y - epsilon);
}
} // namespace gk
