#include "foo.hpp"

void foo_func(App& app) {
	app.video.pixel_buffer->clear(0x00000000);

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
	render::draw_rect(*app.video.pixel_buffer, *app.video.viewport, p1, p2, color);
}

void bar_test() {
	Manager manager;
	// std::cout << "size1: " << manager.dc1_ptr->get_data_size() << std::endl;
	// std::cout << "size2: " << manager.dc2_ptr->get_data_size() << std::endl;
	// std::cout << "size2_small: " << manager.dc2_ptr_small->get_data_size() << std::endl;
	// *manager.dc2_ptr= *manager.dc2_ptr_small;
	// std::cout << "size2: " << manager.dc2_ptr->get_data_size() << std::endl;
	// std::cout << "size2_small: " << manager.dc2_ptr_small->get_data_size() << std::endl;

	std::cout << "size: " << manager.dc1_obj.get_data_size() << std::endl;
	std::cout << "size_small: " << manager.dc1_obj_small.get_data_size() << std::endl;

	manager.dc1_obj = manager.dc1_obj_small;

	std::cout << "size: " << manager.dc1_obj.get_data_size() << std::endl;
	std::cout << "size_small: " << manager.dc1_obj_small.get_data_size() << std::endl;

}

void bar_func() {
	bar_test();
}
