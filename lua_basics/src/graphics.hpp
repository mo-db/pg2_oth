// graphics.hpp
#pragma once
#include "core.hpp"

struct IVec2; // forward declaration
struct Vec2 {
  float x{}, y{};
  Vec2() = default;
  Vec2(const float x_, const float y_) : x{x_}, y{y_} {}
	IVec2 get_IVec2();
  void normalize();
	Vec2 get_orthogonal() const;
  float get_mag() const;
	float get_angle() const;
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
  // void normalize();
	// Vec2 get_orthogonal() const;
  // float get_mag() const;
	// float get_angle() const;
};

IVec2 operator+(const IVec2 &a, const IVec2 &b);
IVec2 operator-(const IVec2 &a, const IVec2 &b);
IVec2 operator*(const IVec2 &v, int d);
IVec2 operator*(int d, const IVec2 &v);

struct Line2 {
	Vec2 p1{}, p2{};
	Line2() = default;
	Line2(const Vec2 p1_, const Vec2 p2_) : p1{p1_}, p2{p2_} {}
	Vec2 get_vector() const;
};

namespace line2 {
Vec2 project_point(const Line2 &line, const Vec2 &P);
bool point_in_segment_bounds(const Line2 &line, const Vec2 &P);
float get_distance_point_ray(const Line2 &line, const Vec2 &P);
float get_distance_point_segment(const Line2 &line, const Vec2 &P);
} // namespace line2
