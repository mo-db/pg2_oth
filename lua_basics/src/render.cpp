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

	std::vector<Pos2> pixels{};
	pixels.reserve((x2 - x1) * (y2 - y1));
	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			pixels.push_back({j, i});
		}
	}
	color_pixels(pixel_buffer, pixels, color);
}

// Vec2 screen_to_world(App& app, const Vec2 &point_screen) {
// 	Vec2 vertex_world = point_screen + .xy_offset;
// 	return vertex_world;
// }
//
Vec2 world_to_screen(Viewport& viewport, const Vec2 &point_world) {
	Vec2 point_screen = (point_world - viewport.xy_offset) * viewport.scale;
	return point_screen;
}
} // namespace render
