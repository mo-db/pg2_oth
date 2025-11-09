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
  ~PixelBuffer() { delete pixels; }
  void clear(uint32_t color);
};

// struct PixelBuffer {
// 	std::unique_ptr<uint32_t> pixels{};
// 	int width = 0;
// 	int height = 0;
//
// 	PixelBuffer(int width_, int height_) : width{width_}, height{height_} {
// 		pixels = std::unique_ptr<uint32_t>(new uint32_t[width_ * height_]);
// 	}
// 	void clear(uint32_t color);
// };

struct Viewport {
  Vec2 xy_offset{};
	float scale{1.0f};
  // Vec2 prev_mouse{};
  // bool panning_active = false;
};

void color_pixels(PixelBuffer& pixel_buffer, std::vector<Pos2>& pixels,
		uint32_t color);
void draw_rect(PixelBuffer& pixel_buffer, Viewport& viewport, Vec2 p1, Vec2 p2,
							 uint32_t color);
void draw_trigon(PixelBuffer* pixel_buffer);
void draw_trigon_line(PixelBuffer* pixel_buffer);
// void draw_circle
// void draw_line

// bool update_pan();
// bool update_zoom();
// Vec2 screen_to_world(const Vec2 &vertex_screen);
Vec2 world_to_screen(Viewport& viewport, const Vec2 &vertex_world);
Vec2 screen_to_world(Viewport& viewport, const Vec2 &point_screen);
} // namespace render
