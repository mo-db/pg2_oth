#include "graphics.hpp"

// ---- Vec2 ----
IVec2 Vec2::get_IVec2() const {
	return {static_cast<int>(std::round(x)),
					static_cast<int>(std::round(y))};
}
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
	return {v.x * d, v.y * d};
}
Vec2 operator/(const Vec2 &v, float d) {
	return {v.x / d, v.y / d};
}
Vec2 operator/(float d, const Vec2 &v) {
	return {d / v.x, d / v.y};
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

// ---- IVec2 ----
Vec2 IVec2::get_Vec2() {
	return {static_cast<float>(x), static_cast<float>(y)};
}
IVec2 operator+(const IVec2 &a, const IVec2 &b) {
	return {a.x + b.x, a.y + b.y};
}
IVec2 operator-(const IVec2 &a, const IVec2 &b) {
	return {a.x - b.x, a.y - b.y};
}
IVec2 operator*(const IVec2 &v, int d) {
	return {v.x * d, v.y * d};
}
IVec2 operator*(int d, const IVec2 &v) {
	return {v.x * d, v.y * d};
}


// --- line ---
Vec2 line_project_point(const Vec2 &line_start, const Vec2& line_end
		, const Vec2 &p) {
	Vec2 a = (line_end - line_start).get_orthogonal();
	double k = ((line_start.x * a.x + line_start.y * a.y) -
							(p.x * a.x + p.y * a.y)) / (a.x * a.x + a.y * a.y);
	return k * a + p;
}
float get_distance_point_ray(const Vec2 &line_start, const Vec2& line_end
		, const Vec2 &p) {
	Vec2 a = (line_end - line_start).get_orthogonal();
	return std::abs((a.x * p.x + a.y * p.y +
									(-a.x * line_start.x - a.y * line_start.y)) / a.get_mag());
}
float get_distance_point_segment(const Vec2& line_start, const Vec2& line_end,
		const Vec2& p) {
  Vec2 projection = line_project_point(line_start, line_end, p);
  if (rect_contains_point(line_start, line_end, projection)) {
    return get_distance_point_ray(line_start, line_end, p);
  } else {
    return std::min(vec2::distance(p, line_start), vec2::distance(p, line_end));
  }
}

// --- rect ---
bool rect_contains_point(Vec2 rect_start, Vec2 rect_end, Vec2 p) {
  float x_min = std::min(rect_start.x, rect_end.x);
  float x_max = std::max(rect_start.x, rect_end.x);
  float y_min = std::min(rect_start.y, rect_end.y);
  float y_max = std::max(rect_start.y, rect_end.y);
  return p.x > x_min && p.x < x_max && p.y > y_min && p.y < y_max;
}

std::vector<Vec2> rect_ray_intersect(Vec2 rect_start, Vec2 rect_end,
                                     Vec2 line_start, Vec2 line_end) {
  float x_min = std::min(rect_start.x, rect_end.x);
  float x_max = std::max(rect_start.x, rect_end.x);
  float y_min = std::min(rect_start.y, rect_end.y);
  float y_max = std::max(rect_start.y, rect_end.y);

  std::vector<Vec2> ixn_points{};

  float dx = line_end.x - line_start.x;
  float dy = line_end.y - line_start.y;
  float m = 0.0f;
  if (!core::equal_epsilon(dx, 0)) {
    m = dy / dx;
  }
  if (!core::equal_epsilon(m, 0)) {
    float ixn_x = (y_min - line_start.y) / m + line_start.x;
    if (ixn_x > x_min && ixn_x < x_max) {
      ixn_points.push_back({ixn_x, y_min});
    }
    float ixn_w = (y_max - line_start.y) / m + line_start.x;
    if (ixn_w > x_min && ixn_w < x_max) {
      ixn_points.push_back({ixn_w, y_max});
    }
  }
  float ixn_y = (x_min - line_start.x) * m + line_start.y;
  if (ixn_y > y_min && ixn_y < y_max) {
    ixn_points.push_back({x_min, ixn_y});
  }
  float ixn_h = (x_max - line_start.x) * m + line_start.y;
  if (ixn_h > y_min && ixn_h < y_max) {
    ixn_points.push_back({x_max, ixn_h});
  }
  return ixn_points;
}

std::vector<Vec2> rect_line_intersect(Vec2 rect_start, Vec2 rect_end,
                                      Vec2 line_start, Vec2 line_end) {
  float x_min = std::min(rect_start.x, rect_end.x);
  float x_max = std::max(rect_start.x, rect_end.x);
  float y_min = std::min(rect_start.y, rect_end.y);
  float y_max = std::max(rect_start.y, rect_end.y);

  std::vector<Vec2> ixn_points{};

  float dx = line_end.x - line_start.x;
  float dy = line_end.y - line_start.y;
  float m = 0.0f;
  if (!core::equal_epsilon(dx, 0)) {
    m = dy / dx;
  }

  if (!core::equal_epsilon(m, 0)) {
    float ixn_x = (y_min - line_start.y) / m + line_start.x;
    if (ixn_x > x_min && ixn_x < x_max &&
        std::min(line_start.y, line_end.y) < y_min &&
        std::max(line_start.y, line_end.y) > y_min) {
      ixn_points.push_back({ixn_x, y_min});
    }
    float ixn_w = (y_max - line_start.y) / m + line_start.x;
    if (ixn_w > x_min && ixn_w < x_max &&
        std::min(line_start.y, line_end.y) < y_max &&
        std::max(line_start.y, line_end.y) > y_max) {
      ixn_points.push_back({ixn_w, y_max});
    }
  }
  float ixn_y = (x_min - line_start.x) * m + line_start.y;
  if (ixn_y > y_min && ixn_y < y_max &&
      std::min(line_start.x, line_end.x) < x_min &&
      std::max(line_start.x, line_end.x) > x_min) {
    ixn_points.push_back({x_min, ixn_y});
  }
  float ixn_h = (x_max - line_start.x) * m + line_start.y;
  if (ixn_h > y_min && ixn_h < y_max &&
      std::min(line_start.x, line_end.x) < x_max &&
      std::max(line_start.x, line_end.x) > x_max) {
    ixn_points.push_back({x_max, ixn_h});
  }
  return ixn_points;
}
