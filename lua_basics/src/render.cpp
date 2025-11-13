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

	// get alpha, beta
	float alpha = p.x / a.x;
	float beta = p.y / b.y;
	float gamma = 1.0f - alpha - beta;

	Bary bary{alpha, beta, gamma};
	return bary;
}


void draw_triangle(PixelBuffer& pixel_buffer, Vert2 vert_a, Vert2 vert_b, Vert2 vert_c) {

	Bary bary{};


	// get min/max x and min/max y
	Vec2 va = vert_a.position;
	Vec2 vb = vert_b.position;
	Vec2 vc = vert_c.position;
	int min_x = std::roundl(std::min(std::min(va.x, vb.x), vc.x));
	int min_y = std::roundl(std::min(std::min(va.y, vb.y), vc.y));
	int max_x = std::roundl(std::max(std::max(va.x, vb.x), vc.x));
	int max_y = std::roundl(std::max(std::max(va.y, vb.y), vc.y));

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
					color_pixels(pixel_buffer, scanline, 0xFFFFFFFF);
				}
			}
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
} // namespace render
