#include "render.hpp"

namespace render {
// need to give integer map instead Vec2

void color_pixels(PixelBuffer& pixel_buffer, std::vector<Pos2>& pixels,
		uint32_t color) {
	for (size_t i = 0; i < pixels.size(); i++) {
		int x = pixels[i].x;
		int y = pixels[i].y;

		if (x >= 0 && y >= 0 && x < pixel_buffer.width &&
				y < pixel_buffer.height) {

			pixel_buffer.pixels[x + y * pixel_buffer.width] = color;
		}
	}
}

void PixelBuffer::clear(uint32_t color) {
	for (int i = 0; i < width * height; i++) { pixels[i] = color; }
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

	std::vector<Pos2> pixels{};
	pixels.reserve((x2 - x1) * (y2 - y1));
	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			pixels.push_back({j, i});
		}
	}
	color_pixels(pixel_buffer, pixels, color);
}

// triangle
Bary calc_brycentric_factors(Vec2 vert_a, Vec2 vert_b, Vec2 vert_c, Vec2 point_p) {
	Vec2 AB = vert_b - vert_a;
	Vec2 BC = vert_c - vert_b;
	Vec2 AP = point_p - vert_a;

	Vec2 a = AB;
	Vec2 b = BC;
	Vec2 p = AP;

	bool alphax_zero = false;
	if (core::equal_epsilon(AB.x, 0.0f)) {
		// switch I. and II.
		a.x = AB.y;
		a.y = AB.x;
		b.x = BC.y;
		b.y = BC.x;
		p.x = AP.y;
		p.y = AP.x;
		alphax_zero = true;
	}
	else if (core::equal_epsilon(AB.y, 0.0f)) {
		alphax_zero = true;
	}

	float t{};

	if (!alphax_zero) {
		// eliminate a.y -> II. * t - I.
		t = a.y / a.x;
		b.y -= (b.x * t);
		p.y -= (p.x * t);
	}

	// eliminate b.x -> I. * t - II.
	t = b.x / b.y;
	p.x -= (p.y * t);

	// TODO: I dont fucking understand why u,v,w are calculated like that
	// get alpha, beta
	float alpha = p.x / a.x;
	float beta = p.y / b.y;
	float gamma = (1.0f - alpha) - beta;

	float u = 1.0f - alpha;
	float v = alpha - beta;
	float w = beta;


	Bary bary{u, v, w};
	// Bary bary{alpha, beta, gamma};
	return bary;
}


void draw_triangle(PixelBuffer& pixel_buffer, Viewport& viewport, Vert2 vert_a, Vert2 vert_b, Vert2 vert_c) {

	Bary bary{};

	// get min/max x and min/max y
	Vec2 va = world_to_screen(viewport, vert_a.position);
	Vec2 vb = world_to_screen(viewport, vert_b.position);
	Vec2 vc = world_to_screen(viewport, vert_c.position);

	int min_x = std::roundl(std::min(std::min(va.x, vb.x), vc.x));
	min_x = std::max(min_x, 0);
	int min_y = std::roundl(std::min(std::min(va.y, vb.y), vc.y));
	min_y = std::max(min_y, 0);
	int max_x = std::roundl(std::max(std::max(va.x, vb.x), vc.x));
	max_x = std::min(max_x, pixel_buffer.width);
	int max_y = std::roundl(std::max(std::max(va.y, vb.y), vc.y));
	max_y = std::min(max_y, pixel_buffer.height);

	std::vector<Pos2> scanline{};
	bool scanline_active = false;
	for (int i = min_y; i < max_y; i++) {
		for (int j = min_x; j < max_x; j++) {
			bary = calc_brycentric_factors(va, vb, vc, Vec2{(float)j, (float)i});

			if (bary.alpha >= 0.0f &&
					bary.beta >= 0.0f &&
					bary.gamma >= 0.0f) {
				if (scanline_active == false) {
					scanline_active = true;
				}
				scanline.push_back(Pos2{j, i});
			} else {
				if (scanline_active) {
					scanline_active = false;
					color_pixels(pixel_buffer, scanline, vert_a.color);
				}
			}
		}
	}
}

// color-modes:
// 1. [Def.] gradient over length
// 2. gradient over width
// 3. 1 color per triangle
// 4. 1 color
void draw_wide_line(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p1, Vec2 p2, float wd, uint8_t color_mode, 
										uint32_t color1, uint32_t color2) {
	Vec2 p_upper;
	Vec2 p_lower;
	if (p1.y > p2.y) {
		p_upper = p1;
		p_lower = p2;
	}
	else {
		p_upper = p2;
		p_lower = p1;
	}

	Vec2 line = p_upper - p_lower;

	Vec2 orthogonal = line.get_orthogonal();
	orthogonal.normalize();

	Vec2 upper_left = p_upper + orthogonal * (wd/2.0f);
	upper_left.x = std::round(upper_left.x);
	upper_left.y = std::round(upper_left.y);

	Vec2 upper_right = upper_left - orthogonal * wd;
	upper_right.x = std::round(upper_right.x);
	upper_right.y = std::round(upper_right.y);

	Vec2 lower_left = upper_left - line;
	lower_left.x = std::round(lower_left.x);
	lower_left.y = std::round(lower_left.y);

	Vec2 lower_right = upper_right - line;
	lower_right.x = std::round(lower_right.x);
	lower_right.y = std::round(lower_right.y);

	draw_triangle(pixel_buffer, viewport, Vert2{color1, upper_left}, 
								Vert2{color1, lower_right}, Vert2{color1, upper_right});
	draw_triangle(pixel_buffer, viewport, Vert2{color2, lower_right}, 
								Vert2{color2, upper_left}, Vert2{color2, lower_left});
}

// floating point line algorithm
void draw_lerp_line(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p0, Vec2 p1, uint32_t color) {

	std::vector<Pos2> pixels{};

	float dx = std::abs(p1.x - p0.x);
	float dy = std::abs(p1.y - p0.y);
	float length = std::max(dx, dy);

	// normal displacement vector for Chebyshev distance
  float unit_displacement_x = dx / length;
  float unit_displacement_y = dy / length;

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
		float x = i * unit_displacement_x + p0.x;
		float y = i * unit_displacement_y + p0.y;


		pixels.push_back(Pos2{static_cast<int>(std::round(x)),
													static_cast<int>(std::round(y))});
	}
	color_pixels(pixel_buffer, pixels, color);
}

void draw_lerp_line_trigon(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p0, Vec2 p1, Vec2 p2, uint32_t color) {

	std::vector<Pos2> pixels{};


	int dx_left = std::abs(p1.x - p0.x);
	int dy_left = std::abs(p1.y - p0.y);
	int sx_left = (p0.x < p1.x) ? 1 : -1;
	int sy_left = (p0.y < p1.y) ? 1 : -1;
	int length_left = std::max(dx_left, dy_left);

	int dx_right = std::abs(p1.x - p0.x);
	int dy_right = std::abs(p1.y - p0.y);
	int sx_right = (p0.x < p1.x) ? 1 : -1;
	int sy_right = (p0.y < p1.y) ? 1 : -1;
	int length_right = std::max(dx_right, dy_right);

	// normal displacement vector for Chebyshev distance
  float x_displace_left = (static_cast<float>(dx_left) / length_left) * sx_left;
  float y_displace_left = (static_cast<float>(dy_left) / length_left) * sy_left;

  float x_displace_right = (static_cast<float>(dx_right) / length_right) * sx_right;
  float y_displace_right = (static_cast<float>(dy_right) / length_right) * sy_right;

	Pos2 left{};
	Pos2 right{};
	int last_y{};

	int index_left{};
	int index_right{};

	for (;;) {
		// left
		last_y = left.y;
		while (left.y == last_y) {
			left.x = static_cast<int>(std::round(index_left * x_displace_left + p0.x));
			left.y = static_cast<int>(std::round(index_left * y_displace_left + p0.y));
			if (index_left >= length_left) { break; }
			index_left++;
		}

		// right
		last_y = right.y;
		while (right.y == last_y) {
			right.x = static_cast<int>(std::round(index_right * x_displace_right + p0.x));
			right.y = static_cast<int>(std::round(index_right * y_displace_right + p0.y));
			if (index_right >= length_right) { break; }
			index_right++;
		}

		// fill
		for (int i = left.x; i <= right.x; i++) {
			pixels.push_back(Pos2{i, left.y});
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

	std::vector<Pos2> pixels{};
	pixels.reserve(2 * std::max(std::abs(dx), std::abs(dy)));

	int epsilon{};
	if (dx > dy) {
		epsilon = (dy - dx) / 2;
		for (;;) {
			pixels.push_back(Pos2{x0, y0});
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
			pixels.push_back(Pos2{x0, y0});
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

	std::vector<Pos2> pixels{};
	pixels.reserve(2 * std::max(std::abs(dx), std::abs(dy)));

	for (;;) {  /* loop */
		pixels.push_back(Pos2{x0, y0});
		if (x0==x1 && y0==y1) break;
		if (err << 1 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (err << 1 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
	color_pixels(pixel_buffer, pixels, color);
}

// top or bottom need to be flat
void bresenham_flat_trigon(PixelBuffer& pixel_buffer, Viewport& viewport,
											Pos2 v0, Pos2 v1, Pos2 v2, uint32_t color) {
	// ---- sort the vertices ----
	Pos2 v_start{};
	Pos2 v_left{};
	Pos2 v_right{};
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

	std::vector<Pos2> pixels{};
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

	Pos2 left{v_start};
	Pos2 right{v_start};

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
			pixels.push_back(Pos2{x, left.y});
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
                 Vec2 p1, Vec2 p2, uint32_t color) {
	// sort for smallest y
  do {
    Vec2 temp = p0;
    if (temp.y > p1.y) {
      p0 = p1;
      p1 = temp;
    }

    if (p2.y < p1.y) {
      temp = p1;
      p1 = p2;
      p2 = temp;
    }
  } while (p0.y > p1.y || p1.y > p2.y);

	Pos2 pos0{};
	pos0.x = std::roundl(p0.x);
	pos0.y = std::roundl(p0.y);

	Pos2 pos1{};
	pos1.x = std::roundl(p1.x);
	pos1.y = std::roundl(p1.y);

	Pos2 pos2{};
	pos2.x = std::roundl(p2.x);
	pos2.y = std::roundl(p2.y);



	if (pos0.y == pos1.y || pos0.y == pos2.y || pos1.y == pos2.y) {
		bresenham_flat_trigon(pixel_buffer, viewport, pos0, pos1, pos2, color);
		return;
	}

	// I draw the middle line twice, but this is cheap so why care
	float k{};
	if (p2.y != p0.y) {
		k = (p1.y - p0.y) / (p2.y - p0.y);
	}
	Vec2 p3 {p0.x + k * (p2.x - p0.x), p1.y};

	Pos2 pos3{};
	pos3.x = std::roundl(p3.x);
	pos3.y = std::roundl(p3.y);

	// fmt::print("###\n0: {}, {}\n1: {}, {}\n2: {}, {}\n3: {}, {}\n\n", 
	// 	pos0.x, pos0.y, pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y);

	bresenham_flat_trigon(pixel_buffer, viewport, pos0, pos1, pos3, color);
	bresenham_flat_trigon(pixel_buffer, viewport, pos1, pos2, pos3, color);
}

void draw_line_wd_new(PixelBuffer& pixel_buffer, Viewport& viewport,
											Vec2 start, Vec2 end, float wd, uint32_t color) {
	Vec2 line = end - start;

	Vec2 a = line.get_orthogonal();
	a.normalize();

	Vec2 p0 = start + a * (wd/2.0f);
	Vec2 p1 = p0 - a * wd;
	Vec2 p2 = p1 + line;
	Vec2 p3 = p0 + line;

	draw_trigon(pixel_buffer, viewport, p0, p1,p3, color);
	draw_trigon(pixel_buffer, viewport, p1, p2,p3, color);

}
void draw_line_wd(PixelBuffer& pixel_buffer, Viewport& viewport,
											Vec2 start, Vec2 end, float wd, uint32_t color) {
	// clamp

	Vec2 line = start - end;

	Vec2 a = line.get_orthogonal();
	a.normalize();

	Vec2 p0 = start + a * (wd/2.0f);
	Vec2 p1 = p0 - a * wd;
	Vec2 p2 = p1 + line;
	Vec2 p3 = p0 + line;
	Vec2 p4{};
	Vec2 p5{};

	float k{};

	if (start.y > end.y) {
		if (p0.y < p1.y) { // p1 -> p4 -> p2, p3 -> p5 -> p0
			k = (-p0.y + p1.y) / line.y; // p4.y == p0.y
			p4 = {p1.x + k * line.x, p0.y};
			p5 = {p3.x + (-k) * line.x, p2.y};
		}
		else {
			k = (-p1.y + p0.y) / line.y; // p4.y == p1.y
			p4 = {p0.x + k * line.x, p1.y};
			p5 = {p2.x + (-k) * line.x, p3.y};
		}
	}
	else {
		if (p0.y < p1.y) {
			k = (p1.y - p0.y) / line.y; // p4.y == p1.y
			p4 = {p0.x + k * line.x, p1.y};
			p5 = {p2.x + (-k) * line.x, p3.y};
		}
		else {
			k = (p0.y - p1.y) / line.y; // p4.y == p0.y
			p4 = {p1.x + k * line.x, p0.y};
			p5 = {p3.x + (-k) * line.x, p2.y};
		}
	}

	Pos2 pos0{};
	pos0.x = std::roundl(p0.y);
	pos0.y = std::roundl(p0.y);

	Pos2 pos1{};
	pos1.x = std::roundl(p1.x);
	pos1.y = std::roundl(p1.y);

	Pos2 pos2{};
	pos2.x = std::roundl(p2.x);
	pos2.y = std::roundl(p2.y);

	Pos2 pos3{};
	pos3.x = std::roundl(p3.x);
	pos3.y = std::roundl(p3.y);

	Pos2 pos4{};
	pos4.x = std::roundl(p4.x);
	pos4.y = std::roundl(p4.y);

	Pos2 pos5{};
	pos5.x = std::roundl(p5.x);
	pos5.y = std::roundl(p5.y);

	bresenham_flat_trigon(pixel_buffer, viewport, pos0, pos1, pos4, color);
	bresenham_flat_trigon(pixel_buffer, viewport, pos2, pos3, pos5, color);
}




Vec2 world_to_screen(Viewport& viewport, const Vec2 &point_world) {
	Vec2 point_screen = (point_world - viewport.xy_offset) * viewport.scale;
	return point_screen;
}

Vec2 screen_to_world(Viewport& viewport, const Vec2 &point_screen) {
	Vec2 point_world = (point_screen / viewport.scale) + viewport.xy_offset;
	return point_world;
}
} // namespace render
