// render
#pragma once

#include "core.hpp"
#include "graphics.hpp"

namespace render {
struct Pos2 {
	int x{};
	int y{};
};

struct PixelBuffer {
	uint32_t *pixels = nullptr;
	int width = 0;
	int height = 0;

	PixelBuffer(int width_, int height_) : width{width_}, height{height_} {
		pixels = new uint32_t[width_ * height_];
	}
	~PixelBuffer() {
		delete pixels;
	}
	void clear(uint32_t color);
};

void color_pixels(PixelBuffer& pixel_buffer, std::vector<Pos2>& pixels,
		uint32_t color);
void draw_rect(PixelBuffer& pixel_buffer, Vec2 p1, Vec2 p2,
							 uint32_t color);
void draw_trigon(PixelBuffer* pixel_buffer);
void draw_trigon_line(PixelBuffer* pixel_buffer);
// void draw_circle
// void draw_line


struct Viewport{
	Vec2 xy_offset{};
	Vec2 prev_mouse{};
	bool panning_active = false;
};

bool update_pan();
// bool update_zoom();
Vec2 screen_to_world(const Vec2 &vertex_screen);
Vec2 world_to_screen(const Vec2 &vertex_world);
} // namespace render
