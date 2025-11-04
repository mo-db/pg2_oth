#include "foo.hpp"

void foo_func(App& app, render::PixelBuffer& pixel_buffer) {
	pixel_buffer.clear(0x00000000);

	static float p1_x = 100;
	static float p1_y = 100;

	static float p2_x = 400;
	static float p2_y = 400;
	
	SDL_Color sdl_color{0, 255, 0, 255};

	if (app.state.input.shift.down()) {
		sdl_color = {255, 255, 255, 255};
		p1_x  += 1.0;
		p2_x  += 1.0;
	}

	if (app.state.input.ctrl.down()) {
		sdl_color = {255, 255, 255, 255};
		p1_x -= 1.0;
		p2_x -= 1.0;
	}

	uint32_t color = SDL_MapRGBA(
			SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr,
			sdl_color.r, sdl_color.g, sdl_color.b, sdl_color.a);

	Vec2 p1 {p1_x, app.state.input.mouse.y};
	Vec2 p2 {p2_x, p2_y};
	render::draw_rect(pixel_buffer, p1, p2, color);
}
