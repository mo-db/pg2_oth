// render
#pragma once

#include "core.hpp"
#include "graphics.hpp"

namespace render {
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

struct Viewport {
  Vec2 xy_offset{};
	float scale{1.0f};
};

// --- base ---
void color_pixels(PixelBuffer& pixel_buffer, std::vector<IVec2>& pixels,
		uint32_t color);
Vec2 world_to_screen(Viewport& viewport, const Vec2 &vertex_world);
Vec2 screen_to_world(Viewport& viewport, const Vec2 &point_screen);

// --- drawing functions ---
void draw_rect(PixelBuffer& pixel_buffer, Viewport& viewport, Vec2 p1, Vec2 p2,
							 uint32_t color);

void draw_lerp_line(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p1, Vec2 p2, uint32_t color);
void bresenham_line(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p0, Vec2 p1, uint32_t color);
void bresenham_line2(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p0, Vec2 p1, uint32_t color);

void draw_trigon(PixelBuffer &pixel_buffer, Viewport &viewport, Vec2 p0,
                 Vec2 p1, Vec2 p2, uint32_t color, uint8_t impl);
void draw_lerp_line_trigon(PixelBuffer &pixel_buffer, IVec2 p0,
                           IVec2 p1, IVec2 p2, uint32_t color);

void draw_thick_line(PixelBuffer& pixel_buffer, Viewport& viewport,
											Vec2 start, Vec2 end, float wd, uint32_t color);





// void draw_circle

// bool update_pan();
// bool update_zoom();
// Vec2 screen_to_world(const Vec2 &vertex_screen);

} // namespace render
