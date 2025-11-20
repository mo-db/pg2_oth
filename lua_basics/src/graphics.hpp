// graphics.hpp
#pragma once
#include "core.hpp"

struct IVec2; // forward declaration
struct Vec2 {
  float x{}, y{};
  Vec2() = default;
  Vec2(const float x_, const float y_) : x{x_}, y{y_} {}
  void normalize();
	Vec2 get_norm() const; // TODO
	Vec2 get_orthogonal() const;
	IVec2 get_IVec2() const;
  float get_mag() const;
	float get_angle() const; // TODO
};

Vec2 operator+(const Vec2 &a, const Vec2 &b);
Vec2 operator-(const Vec2 &a, const Vec2 &b);
Vec2 operator*(const Vec2 &v, float d);
Vec2 operator*(float d, const Vec2 &v);
Vec2 operator/(const Vec2 &v, float d);
Vec2 operator/(float d, const Vec2 &v);

namespace vec2 {
float dot(const Vec2 &a, const Vec2 &b);
float distance(const Vec2 &a, const Vec2 &b);
bool equal_epsilon(const Vec2 &a, const Vec2 &b);
} // namespace vec2

struct IVec2 {
  int x{}, y{};
  IVec2() = default;
  IVec2(const int x_, const int y_) : x{x_}, y{y_} {}
	Vec2 get_Vec2();
  // void normalize(); // TDOD
	// IVec2 get_norm() const; // TODO
  // void normalize();
	// Vec2 get_orthogonal() const;
  // float get_mag() const;
	// float get_angle() const;
};

IVec2 operator+(const IVec2 &a, const IVec2 &b);
IVec2 operator-(const IVec2 &a, const IVec2 &b);
IVec2 operator*(const IVec2 &v, int d);
IVec2 operator*(int d, const IVec2 &v);

// --- line ---
Vec2 line_project_point(const Vec2 &line_start, const Vec2 &line_end, const Vec2 &p);
float get_distance_point_ray(const Vec2 &line_start, const Vec2 &line_end,
                             const Vec2 &p);
float get_distance_point_segment(const Vec2 &line_start, const Vec2 &line_end,
                                 const Vec2 &p);
// --- rect ---
bool rect_contains_point(Vec2 rect_start, Vec2 rect_end, Vec2 p);
std::vector<Vec2> rect_ray_intersect(Vec2 rect_start, Vec2 rect_end,
                                     Vec2 line_start, Vec2 line_end);
std::vector<Vec2> rect_line_intersect(Vec2 rect_start, Vec2 rect_end,
                                      Vec2 line_start, Vec2 line_end);
