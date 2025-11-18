#include "render.hpp"

// world_to_screen conversion is done for the final vertexes to be drawn

namespace render {

bool inside_rect(Vec2 rect_start, Vec2 rect_end, Vec2 p) {
  float x_min = std::min(rect_start.x, rect_end.x);
  float x_max = std::max(rect_start.x, rect_end.x);
  float y_min = std::min(rect_start.y, rect_end.y);
  float y_max = std::max(rect_start.y, rect_end.y);
  return p.x > x_min && p.x < x_max && p.y > y_min && p.y < y_max;
}

// there can be max of 2 ixn points
std::vector<Vec2>
	linesegment_rect_intersect(Vec2 rect_start, Vec2 rect_end, Vec2 line_start, Vec2 line_end) {
	float x_min = std::min(rect_start.x, rect_end.x);
	float x_max = std::max(rect_start.x, rect_end.x);
	float y_min = std::min(rect_start.y, rect_end.y);
	float y_max = std::max(rect_start.y, rect_end.y);

	std::vector<Vec2> ixn_points{};

	float m = (line_end.y - line_start.y) / (line_end.x - line_start.x);
	float ixn_x = (y_min - line_start.y) / m + line_start.x;
	if (ixn_x > x_min && ixn_x < x_max && 
			std::min(line_start.y, line_end.y) < y_min && 
			std::max(line_start.y, line_end.y) > y_min) {
		ixn_points.push_back({ixn_x, y_min});
	}
	float ixn_y = (x_min - line_start.x) * m + line_start.y;
	if (ixn_y > y_min && ixn_y < y_max && 
			std::min(line_start.x, line_end.x) < x_min && 
			std::max(line_start.x, line_end.x) > x_min) {
		ixn_points.push_back({x_min, ixn_y});
	}
	float ixn_w = (y_max - line_start.y) / m + line_start.x;
	if (ixn_w > x_min && ixn_w < x_max && 
			std::min(line_start.y, line_end.y) < y_max && 
			std::max(line_start.y, line_end.y) > y_max) {
		ixn_points.push_back({ixn_w, y_max});
	}
	float ixn_h = (x_max - line_start.x) * m + line_start.y;
	if (ixn_h > y_min && ixn_h < y_max && 
			std::min(line_start.x, line_end.x) < x_max && 
			std::max(line_start.x, line_end.x) > x_max) {
		ixn_points.push_back({x_max, ixn_h});
	}
	return ixn_points;
}


void PixelBuffer::clear(uint32_t color) {
	for (int i = 0; i < width * height; i++) { pixels[i] = color; }
}

void color_pixels(PixelBuffer &pixel_buffer, std::vector<IVec2> &pixels,
                  uint32_t color) {
  for (size_t i = 0; i < pixels.size(); i++) {
    int x = pixels[i].x;
    int y = pixels[i].y;
    if (x >= 0 && y >= 0 && x < pixel_buffer.width && y < pixel_buffer.height) {
      pixel_buffer.pixels[x + y * pixel_buffer.width] = color;
    }
  }
}

Vec2 world_to_screen(Viewport& viewport, const Vec2 &point_world) {
	Vec2 point_screen = (point_world - viewport.xy_offset) * viewport.scale;
	return point_screen;
}

Vec2 screen_to_world(Viewport& viewport, const Vec2 &point_screen) {
	Vec2 point_world = (point_screen / viewport.scale) + viewport.xy_offset;
	return point_world;
}

void draw_rect(PixelBuffer& pixel_buffer, Viewport& viewport, Vec2 p1, Vec2 p2, uint32_t color) {
	p1 = world_to_screen(viewport, p1);
	p2 = world_to_screen(viewport, p2);

	int x1 = std::lround(std::max(std::min(p1.x, p2.x), 0.f));
	int x2 = std::lround(std::min(std::max(p1.x, p2.x), (float)pixel_buffer.width));
	int y1 = std::lround(std::max(std::min(p1.y, p2.y), 0.f));
	int y2 = std::lround(std::min(std::max(p1.y, p2.y), (float)pixel_buffer.height));

	if (x1 > pixel_buffer.width || x2 < 0.0f ||
			y1 > pixel_buffer.height || y2 < 0.0f ) {
		return;
	}

	std::vector<IVec2> pixels{};
	pixels.reserve((x2 - x1) * (y2 - y1));
	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			pixels.push_back({j, i});
		}
	}
	color_pixels(pixel_buffer, pixels, color);
}

// floating point line algorithm
void draw_lerp_line(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p0, Vec2 p1, uint32_t color) {

	IVec2 v0 = world_to_screen(viewport, p0).get_IVec2();
	IVec2 v1 = world_to_screen(viewport, p1).get_IVec2();
	IVec2 vertex{};

	int dx = std::abs(v1.x - v0.x);
	int dy = std::abs(v1.y - v0.y);
	int length = std::max(dx, dy);

	std::vector<IVec2> pixels{};
	pixels.reserve(length);

	// normal displacement vector for Chebyshev distance
  float x_displace = static_cast<float>(dx) / length;
  float y_displace = static_cast<float>(dy) / length;

	for (int i = 0; i < length; i++) {
		// float t = static_cast<float>(i) / static_cast<float>(length);

		// parametric line can be written in two forms:
		// l(t) = start_point + t * v(start_point, end_point)
		// float x = t * static_cast<float>(delta_x) + static_cast<float>(start.x);
		// float y = t * static_cast<float>(delta_y) + static_cast<float>(start.y);

		// l(t) = (1 - t) * start_point + t * end_point
		// float x = (1.0f - t) * static_cast<float>(start.x) + t * static_cast<float>(end.x);
		// float y = (1.0f - t) * static_cast<float>(start.y) + t * static_cast<float>(end.y);

		// use normal vector to avoid division every step
		vertex.x = static_cast<int>(std::round(i * x_displace + v0.x));
		vertex.y = static_cast<int>(std::round(i * y_displace + v0.y));
		pixels.push_back(vertex);
	}
	color_pixels(pixel_buffer, pixels, color);
}
void draw_lerp_line_trigon2(PixelBuffer &pixel_buffer, Viewport &viewport,
                            IVec2 v0, IVec2 v1, IVec2 v2, uint32_t color) {
  bool switched_xy{false};
  if (v0.x == v1.x || v0.x == v2.x || v1.x == v2.x) {
		switched_xy = true;
		int temp = v0.x;
		v0.x = v0.y;
		v0.y = temp;
		temp = v1.x;
		v1.x = v1.y;
		v1.y = temp;
		temp = v2.x;
		v2.x = v2.y;
		v2.y = temp;
  } else if (v0.y == v1.y || v0.y == v2.y || v1.y == v2.y) {
		// do nothing
  } else {
    throw std::runtime_error("Trigon is not flat!");
  }
	fmt::print("v0: {}, {}\nv1: {}, {}\nv2 {}, {}\n", v0.x, v0.y, v1.x, v1.y,
			v2.x, v2.y);
	fmt::print("switched: {}\n", switched_xy);

	// ---- sort the vertices ----
	IVec2 v_start{};
	IVec2 v_left{};
	IVec2 v_right{};
	if (v0.y == v1.y) { 
		v_start = v2; 
		if (v0.x < v1.x) {
			v_left = v0;
			v_right = v1;
		}
		else {
			v_left = v1;
			v_right = v0;
		}
	}
	else if (v0.y == v2.y) { 
		v_start = v1;
		if (v0.x < v2.x) {
			v_left = v0;
			v_right = v2;
		}
		else {
			v_left = v2;
			v_right = v0;
		}
	}
	else if (v1.y == v2.y) {
		v_start = v0;
		if (v1.x < v2.x) {
			v_left = v1;
			v_right = v2;
		}
		else {
			v_left = v2;
			v_right = v1;
		}
	}

	int dx_left = std::abs(v_left.x - v_start.x);
	int dy_left = std::abs(v_left.y - v_start.y);
	int sx_left = (v_start.x < v_left.x) ? 1 : -1;
	int sy_left = (v_start.y < v_left.y) ? 1 : -1;
	int length_left = std::max(dx_left, dy_left);

	int dx_right = std::abs(v_right.x - v_start.x);
	int dy_right = std::abs(v_right.y - v_start.y);
	int sx_right = (v_start.x < v_right.x) ? 1 : -1;
	int sy_right = (v_start.y < v_right.y) ? 1 : -1;
	int length_right = std::max(dx_right, dy_right);

	std::vector<IVec2> pixels{};
	pixels.reserve(dy_left * (dx_left + dx_right));

	// normal displacement vector for Chebyshev distance
  float x_displace_left = (static_cast<float>(dx_left) / length_left) * sx_left;
  float y_displace_left = (static_cast<float>(dy_left) / length_left) * sy_left;

  float x_displace_right = (static_cast<float>(dx_right) / length_right) * sx_right;
  float y_displace_right = (static_cast<float>(dy_right) / length_right) * sy_right;

	IVec2 left{};
	IVec2 right{};
	int last_y{};

	int index_left{};
	int index_right{};

	for (;;) {
		// left
		last_y = left.y;
		while (left.y == last_y) {
			left.x = static_cast<int>(std::round(index_left * x_displace_left + v_start.x));
			left.y = static_cast<int>(std::round(index_left * y_displace_left + v_start.y));
			if (index_left >= length_left) { break; }
			index_left++;
		}

		// right
		last_y = right.y;
		while (right.y == last_y) {
			right.x = static_cast<int>(std::round(index_right * x_displace_right + v_start.x));
			right.y = static_cast<int>(std::round(index_right * y_displace_right + v_start.y));
			if (index_right >= length_right) { break; }
			index_right++;
		}

		// fill
		for (int i = left.x; i <= right.x; i++) {
			if (switched_xy) {
				pixels.push_back(IVec2{left.y, i});
			}
			pixels.push_back(IVec2{i, left.y});
		}

		if (index_left == length_left || index_right == length_right) { break; }
	}
	color_pixels(pixel_buffer, pixels, color);
}

// my bresenham algorythm, i still don't understand epsilon init value
void bresenham_line(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p0, Vec2 p1, uint32_t color) {
	// round to nearest integer
	int x0 = std::lround(p0.x);
	int y0 = std::lround(p0.y);
	int x1 = std::lround(p1.x);
	int y1 = std::lround(p1.y);

	// clamp to screen dimentsion
	x0 = std::min(std::max(x0, 0), std::min(x0, pixel_buffer.width));
	y0 = std::min(std::max(y0, 0), std::min(y0, pixel_buffer.height));
	x1 = std::min(std::max(x1, 0), std::min(x1, pixel_buffer.width));
	y1 = std::min(std::max(y1, 0), std::min(y1, pixel_buffer.height));

	int dx = std::abs(x1-x0) * 2; // *2 to be able to do /2 later
	int dy = std::abs(y1-y0) * 2; // *2 to be able to do /2 later
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;

	std::vector<IVec2> pixels{};
	pixels.reserve(2 * std::max(std::abs(dx), std::abs(dy)));

	int epsilon{};
	if (dx > dy) {
		epsilon = (dy - dx) / 2;
		for (;;) {
			pixels.push_back(IVec2{x0, y0});
			if (x0 == x1 && y0 == y1) { break; }
			epsilon += dy;
			if (epsilon << 1 >= dx) {
				y0 += sy;
				epsilon -= dx;
			}
			x0 += sx;
		}
	}
	else {
		epsilon = (dx - dy) / 2;
		for (;;) {
			pixels.push_back(IVec2{x0, y0});
			if (x0 == x1 && y0 == y1) { break; }
			epsilon += dx;
			if (epsilon << 1 >= dy) {
				x0 += sx;
				epsilon -= dy;
			}
			y0 += sy;
		}
	}

	color_pixels(pixel_buffer, pixels, color);
}

// zingles bresenham algorithm, no octant calculations needed
void bresenham_line2(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p0, Vec2 p1, uint32_t color) {
	// round to nearest integer
	int x0 = std::lround(p0.x);
	int y0 = std::lround(p0.y);
	int x1 = std::lround(p1.x);
	int y1 = std::lround(p1.y);

	// clamp to screen dimentsion
	x0 = std::min(std::max(x0, 0), std::min(x0, pixel_buffer.width));
	y0 = std::min(std::max(y0, 0), std::min(y0, pixel_buffer.height));
	x1 = std::min(std::max(x1, 0), std::min(x1, pixel_buffer.width));
	y1 = std::min(std::max(y1, 0), std::min(y1, pixel_buffer.height));

	int dx =  abs(x1-x0);
	int dy = -abs(y1-y0); // ? neg because upper left orientation?
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;

	// ? Why dx+dy as error init
	int err = dx+dy, e2; /* error value e_xy */ 

	std::vector<IVec2> pixels{};
	pixels.reserve(2 * std::max(std::abs(dx), std::abs(dy)));

	for (;;) {  /* loop */
		pixels.push_back(IVec2{x0, y0});
		if (x0==x1 && y0==y1) break;
		if (err << 1 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (err << 1 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
	color_pixels(pixel_buffer, pixels, color);
}

// top or bottom need to be flat
void bresenham_flat_trigon(PixelBuffer& pixel_buffer, Viewport& viewport,
											IVec2 v0, IVec2 v1, IVec2 v2, uint32_t color) {
	// ---- sort the vertices ----
	IVec2 v_start{};
	IVec2 v_left{};
	IVec2 v_right{};
	if (v0.y == v1.y) { 
		v_start = v2; 
		if (v0.x < v1.x) {
			v_left = v0;
			v_right = v1;
		}
		else {
			v_left = v1;
			v_right = v0;
		}
	}
	else if (v0.y == v2.y) { 
		v_start = v1;
		if (v0.x < v2.x) {
			v_left = v0;
			v_right = v2;
		}
		else {
			v_left = v2;
			v_right = v0;
		}
	}
	else if (v1.y == v2.y) {
		v_start = v0;
		if (v1.x < v2.x) {
			v_left = v1;
			v_right = v2;
		}
		else {
			v_left = v2;
			v_right = v1;
		}
	}
	else { throw std::runtime_error("Trigon is not flat!"); }

	// fmt::print("start: {}, {}\n left: {}, {}\nright: {}, {}\n", v_start.x, v_start.y,
	// 		v_left.x, v_left.y, v_right.x, v_right.y);

	// ---- Bresenham ----
	int sx_left = v_start.x < v_left.x ? 1 : -1;
	int sx_right = v_start.x < v_right.x ? 1 : -1;
	int sy = v_start.y < v_left.y ? 1 : -1;

	int dx_left = std::abs(v_left.x - v_start.x);
	int dx_right = std::abs(v_right.x - v_start.x);
	int dy = std::abs(v_left.y - v_start.y); 

	if (dx_left == 0 || dx_right == 0 || dy == 0) {
		return;
	}

	std::vector<IVec2> pixels{};
	pixels.reserve(dy * (dx_left + dx_right));

	int err_left{0};
	int err_right{0};

	// ---- swap ----
	bool left_swapped{false};
	bool right_swapped{false};
	if (dx_left > dy) {
		left_swapped = true;
	}
	if (dx_right > dy) {
		right_swapped = true;
	}

	IVec2 left{v_start};
	IVec2 right{v_start};

	while(1) {
		// ---- left line ----
		if (left_swapped) { // dy is slow
			while(1) {
				// fmt::print("###1###\n");
				err_left += dy;
				left.x += sx_left;
				if (2*err_left >= dx_left) {
					left.y += sy;
					err_left -= dx_left;
					break;
				}
			}
		}
		else {
			err_left += dx_left;
			left.y += sy;
			if (2*err_left >= dy) {
				left.x += sx_left;
				err_left -= dy;
			}
		}

		// ---- right line ----
		if (right_swapped) { // dy is slow
			while(1) {
				// fmt::print("###2###\n");
				err_right += dy;
				right.x += sx_right;
				if (2*err_right >= dx_right) {
					right.y += sy;
					err_right -= dx_right;
					break;
				}
			}
		}
		else {
			err_right += dx_right;
			right.y += sy;
			if (2*err_right >= dy) {
				right.x += sx_right;
				err_right -= dy;
			}
		}

		// ---- scanline fill ----
		// fmt::print("left_x {}, right_x {}\n", left.x, right.x);
		pixels.push_back(left);
		int x = left.x + 1;
		for (; x <= right.x && x < pixel_buffer.width; x++) {
			// fmt::print("###3###\n");
			pixels.push_back(IVec2{x, left.y});
		}

		if (left.x < 0 || right.x < 0) {
			fmt::print("### WTF < 0 ###\n");
			break;
		}
		if (left.x > pixel_buffer.width || right.x > pixel_buffer.width) {
			fmt::print("### WTF > width ###\n");
			break;
		}
		if (right.x == v_right.x && right.y == v_right.y) { break; }
		if (left.x == v_left.x && left.y == v_left.y) { break; }
		// fmt::print("###4###\n");
	} // end

	color_pixels(pixel_buffer, pixels, color);
}

void draw_trigon(PixelBuffer &pixel_buffer, Viewport &viewport, Vec2 p0,
                 Vec2 p1, Vec2 p2, uint32_t color, uint8_t impl) {

	p0 = world_to_screen(viewport, p0);
	p1 = world_to_screen(viewport, p1);
	p2 = world_to_screen(viewport, p2);

	// define screen borders
	Vec2 rect_start{100.0f, 100.0f};
	Vec2 rect_end{static_cast<float>(pixel_buffer.width) - 100.0f,
								static_cast<float>(pixel_buffer.height) - 100.0f};
	// all relevant points to build trigons
	std::vector<Vec2> fpoints{};

	// calculate ixn_points with screen borders
	std::vector<Vec2> ixn_points0 = 
		linesegment_rect_intersect(rect_start, rect_end, p0, p1);
	std::vector<Vec2> ixn_points1 = 
		linesegment_rect_intersect(rect_start, rect_end, p0, p2);
	std::vector<Vec2> ixn_points2 = 
		linesegment_rect_intersect(rect_start, rect_end, p1, p2);

	// add ixn_points to fpoints
	for (size_t i = 0; i < ixn_points0.size(); i++) {
		fpoints.push_back(ixn_points0[i]);
	}
	for (size_t i = 0; i < ixn_points1.size(); i++) {
		fpoints.push_back(ixn_points1[i]);
	}
	for (size_t i = 0; i < ixn_points2.size(); i++) {
		fpoints.push_back(ixn_points2[i]);
	}

	// add points inside rect borders to fpoints
	if (inside_rect(rect_start, rect_end, p0)) {
		fpoints.push_back(p0);
	}
	if (inside_rect(rect_start, rect_end, p1)) {
		fpoints.push_back(p1);
	}
	if (inside_rect(rect_start, rect_end, p2)) {
		fpoints.push_back(p2);
	}

	// round all points to nerest int
	std::vector<IVec2> points{};
	for (size_t i = 0; i < fpoints.size(); i++) {
		points.push_back(world_to_screen(viewport, fpoints[i]).get_IVec2());
	}

	fmt::print("points.size(): {}\n", points.size());

	struct ITrigon {
		IVec2 p0;
		IVec2 p1;
		IVec2 p2;
	};
	std::vector<ITrigon> trigons{};

	// ---- split quad into trigons ----
	if (points.size() == 4) {
		Vec2 centeroid{};
		for (int i = 0; i < 4; i++) {
			centeroid.x += points[i].x;
			centeroid.y += points[i].y;
		}
		centeroid = centeroid * 0.25;

    std::array<int,4> idx = {0,1,2,3};
    auto angle = [&](int i){ 
			Vec2 v = points[i].get_Vec2() - centeroid;
			return std::atan2(v.y, v.x); 
		};
    std::sort(idx.begin(), idx.end(), [&](int a, int b){ return angle(a) < angle(b); });

		trigons.push_back(ITrigon{points[idx[0]], points[idx[2]], points[idx[1]]});
		trigons.push_back(ITrigon{points[idx[0]], points[idx[2]], points[idx[3]]});
	} else {
		trigons.push_back(ITrigon{points[0], points[1], points[2]});
	}

	// ---- draw all trigons ----
	for (size_t i = 0; i < trigons.size(); i++) {
		ITrigon& trigon = trigons[i];

		// early continue if trigon has no area
		if ((trigon.p0.x == trigon.p1.x && trigon.p0.y == trigon.p1.y) ||
				(trigon.p0.x == trigon.p2.x && trigon.p0.y == trigon.p2.y) ||
				(trigon.p1.x == trigon.p2.x && trigon.p1.y == trigon.p2.y)) {
			continue;
		}

		// if allready flat, draw right away
		if (trigon.p0.x == trigon.p1.x ||
				trigon.p0.x == trigon.p2.x ||
				trigon.p1.x == trigon.p2.x ||
				trigon.p0.y == trigon.p1.y ||
				trigon.p0.y == trigon.p2.y ||
				trigon.p1.y == trigon.p2.y) {
			draw_lerp_line_trigon2(pixel_buffer, viewport, trigon.p0,
														trigon.p1, trigon.p2, color);
		} else {
		// sort for smallest y coordinate
			do {
				IVec2 temp = trigon.p0;
				if (temp.y > trigon.p1.y) {
					trigon.p0 = trigon.p1;
					trigon.p1 = temp;
				}

				if (trigon.p2.y < trigon.p1.y) {
					temp = trigon.p1;
					trigon.p1 = trigon.p2;
					trigon.p2 = temp;
				}
			} while (trigon.p0.y > trigon.p1.y || trigon.p1.y > trigon.p2.y);

			// split horizontally, draw both splits
			float k = static_cast<float>(trigon.p1.y - trigon.p0.y) /
								(trigon.p2.y - trigon.p0.y);
			IVec2 p3 {static_cast<int>(std::round(trigon.p0.x + k * 
								(trigon.p2.x - trigon.p0.x))), trigon.p1.y};
			draw_lerp_line_trigon2(pixel_buffer, viewport, trigon.p0,
														trigon.p1, p3, color);
			draw_lerp_line_trigon2(pixel_buffer, viewport, trigon.p2,
														trigon.p1, p3, color);
		}
	}
}

void draw_thick_line(PixelBuffer& pixel_buffer, Viewport& viewport,
											Vec2 start, Vec2 end, float wd, uint32_t color) {
	Vec2 line = end - start;

	Vec2 a = line.get_orthogonal();
	a.normalize();

	Vec2 p0 = start + a * (wd/2.0f);
	Vec2 p1 = p0 - a * wd;
	Vec2 p2 = p1 + line;
	Vec2 p3 = p0 + line;

	draw_trigon(pixel_buffer, viewport, p0, p1,p3, color, 0);
	draw_trigon(pixel_buffer, viewport, p1, p2,p3, color, 0);
}
} // namespace render
