#include "graphics.hpp"

// ---- Vec2 ----
void Vec2::normalize() { 
	float m = get_mag(); x /= m; y /= m;
}

Vec2 Vec2::get_orthogonal() const { 
	return {-y, x}; 
}

float Vec2::get_mag() const { 
	return std::sqrt(x * x + y * y);
}

float Vec2::get_angle() const {
	float angle = -std::atan2(y, x);
	if (angle < 0) { angle += 2 * core::pi; }
	return angle;
}

Vec2 operator+(const Vec2 &a, const Vec2 &b) {
	return {a.x + b.x, a.y + b.y};
}
Vec2 operator-(const Vec2 &a, const Vec2 &b) {
	return {a.x - b.x, a.y - b.y};
}
Vec2 operator*(const Vec2 &v, float d) {
	return {v.x * d, v.y * d};
}
Vec2 operator*(float d, const Vec2 &v) {
	return v * d;
}

namespace vec2 {
float dot(const Vec2 &a, const Vec2 &b) {
	return a.x * b.x + a.y * b.y;
}
float distance(const Vec2 &a, const Vec2 &b) {
  return std::sqrt(std::pow(a.x - b.x, 2.0) + std::pow(a.y - b.y, 2.0));
}
bool equal_epsilon(const Vec2 &a, const Vec2 &b) {
	if (core::equal_epsilon(a.x, b.x) && core::equal_epsilon(a.y, b.y)) {
		return true;
	} else {
		return false;
	}
}
} // namespace vec2

// ---- Line2 ----

Vec2 Line2::get_vector() const {
	return p2 - p1;
}

namespace line2 {
Vec2 project_point(const Line2 &line, const Vec2 &p) {
	Vec2 a = line.get_vector().get_orthogonal();
	double k = ((line.p1.x * a.x + line.p1.y * a.y) -
							(p.x * a.x + p.y * a.y)) / (a.x * a.x + a.y * a.y);
	return k * a + p;
}

bool point_in_segment_bounds(const Line2 &line, const Vec2 &p) {
  double distance_to_far_endpoint = std::max(vec2::distance(line.p1, p),
																						 vec2::distance(line.p2, p));
  return distance_to_far_endpoint <= vec2::distance(line.p1, line.p2);
}

float get_distance_point_ray(const Line2 &line, const Vec2 &P) {
	Vec2 a = line.get_vector().get_orthogonal();
	return std::abs((a.x * P.x + a.y * P.y +
									(-a.x * line.p1.x - a.y * line.p1.y)) / a.get_mag());
}
float get_distance_point_segment(const Line2 &line, const Vec2 &P) {
  Vec2 projected_point = project_point(line, P);
  if (point_in_segment_bounds(line, projected_point)) {
    return get_distance_point_ray(line, P);
  } else {
    return std::min(vec2::distance(P, line.p1), vec2::distance(P, line.p2));
  }
}
} // namespace line2
