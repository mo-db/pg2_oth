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

void draw_rect(PixelBuffer& pixel_buffer, Vec2 p1, Vec2 p2, uint32_t color) {
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
} // namespace render
