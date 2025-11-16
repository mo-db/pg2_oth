// render
#pragma once

#include "core.hpp"
#include "graphics.hpp"

namespace render {
struct Pos2 {
  int x{};
  int y{};
};

struct Vert2 {
	uint32_t color{};
	Vec2 position{};
};

struct Bary {
	float alpha{};
	float beta{};
	float gamma{};
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

Bary calc_brycentric_factors(Vec2 vert_a, Vec2 vert_b, Vec2 vert_c, Vec2 point_p);



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
void draw_wide_line(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p1, Vec2 p2, float wd, uint8_t color_mode, 
										uint32_t color1, uint32_t color2);
// test
void draw_lerp_line(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p1, Vec2 p2, uint32_t color);
void bresenham_line(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p0, Vec2 p1, uint32_t color);
void bresenham_line2(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 p0, Vec2 p1, uint32_t color);
void draw_line_wd_new(PixelBuffer& pixel_buffer, Viewport& viewport, 
		 								Vec2 start, Vec2 end, float wd, uint32_t color);
void draw_trigon(PixelBuffer &pixel_buffer, Viewport &viewport, Vec2 p0,
                 Vec2 p1, Vec2 p2, uint32_t color);

void draw_triangle(PixelBuffer& pixel_buffer, Viewport& viewport, Vert2 vert_a, Vert2 vert_b, Vert2 vert_c);

void color_pixels(PixelBuffer& pixel_buffer, std::vector<Pos2>& pixels,
		uint32_t color);
void draw_rect(PixelBuffer& pixel_buffer, Viewport& viewport, Vec2 p1, Vec2 p2,
							 uint32_t color);
// void draw_trigon(PixelBuffer* pixel_buffer);
void draw_trigon_line(PixelBuffer* pixel_buffer);
// void draw_circle
// void draw_line

// bool update_pan();
// bool update_zoom();
// Vec2 screen_to_world(const Vec2 &vertex_screen);
Vec2 world_to_screen(Viewport& viewport, const Vec2 &vertex_world);
Vec2 screen_to_world(Viewport& viewport, const Vec2 &point_screen);
} // namespace render
