// render_types.hpp

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
  Vec2 prev_mouse{};
  bool panning_active = false;
};
} // namespace render
