#include "foo.hpp"

// states as functions
// have boolean and if-conditions for enter/exit
// return state_var

void rect_sm(App& app, std::vector<SDL_FRect>& rects) {
	enum State {
		DRAWING,
		NOT,
	};
	static State state = NOT;
	Vec2 mouse =
		render::screen_to_world(*app.video.viewport, app.state.input.mouse);

	switch (state) {
		case DRAWING: {
			SDL_FRect& rect = rects.back();
			rect.w = mouse.x - rect.x;
			rect.h = mouse.y - rect.y;
			if (app.state.input.mouse_left.just_pressed()) {
				state = NOT;
			}
			break;
		}
		case NOT: {
			if (app.state.input.mouse_left.just_pressed()) {
				rects.push_back(SDL_FRect{mouse.x, mouse.y, 0.0f, 0.0f});
				state = DRAWING;
			}
			break;
		}
	}
}

void foo_func(App& app) {
	static std::vector<SDL_FRect> rects{};
	rect_sm(app, rects);


	app.video.pixel_buffer->clear(0x00000000);

	SDL_Color sdl_color{0, 255, 0, 255};

	uint32_t color = SDL_MapRGBA(
			SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr,
			sdl_color.r, sdl_color.g, sdl_color.b, sdl_color.a);

	Vec2 mouse =
		render::screen_to_world(*app.video.viewport, app.state.input.mouse);

	render::Vert2 vert_a{color, Vec2{100.0f, 100.0f}};
	render::Vert2 vert_b{color, Vec2{300.0f, 200.0f}};
	render::Vert2 vert_c{color, mouse};
	// render::Bary bary = render::calc_brycentric_factors(vert_a.position, vert_b.position, vert_c.position, mouse);
	// fmt::print("alpha: {}f, beta: {}f, gamma: {}f\n", bary.alpha, bary.beta, bary.gamma);
	// draw_triangle(*app.video.pixel_buffer, *app.video.viewport, vert_a, vert_b, vert_c);

	for (size_t i = 0; i < rects.size(); i++) {
		Vec2 p1 {rects[i].x, rects[i].y};
		Vec2 p2 {rects[i].x + rects[i].w, rects[i].y + rects[i].h};
		render::draw_rect(*app.video.pixel_buffer, *app.video.viewport, p1, p2, color);
	}

	Vec2 off_mouse{mouse.x+8.0f, mouse.y+8.0f};
	// draw_wide_line(*app.video.pixel_buffer, *app.video.viewport, 
	// 	 								Vec2{100, 100}, mouse, 2.0f, 1, 
	// 									0xFFFF00FF, 0xFF00FFFF);
	// draw_lerp_line(*app.video.pixel_buffer, *app.video.viewport, 
	// 							 Vec2{150, 100}, mouse, 0xFFFFFFFF);
	// bresenham_line(*app.video.pixel_buffer, *app.video.viewport, 
	// 							 {100.0, 100.0}, mouse, 0xFF00FF00);
	// bresenham_line2(*app.video.pixel_buffer, *app.video.viewport, 
	// 							 {108.0, 108.0}, off_mouse, 0xFF0000FF);



	float rn0 = SDL_randf();
	float rn1 = SDL_randf();
	// float rn2 = SDL_randf();


	float w = (float)(*app.video.pixel_buffer).width;
	float h = (float)(*app.video.pixel_buffer).height;
	
	Vec2 v0{300.0f, 300.0f};
	Vec2 v1{700.0f, 400.0f};
	Vec2 v2{750.0f, 600.0f};

	const auto start = core::Clock::now();
	// for (int i = 0; i < 20; i++) {
	// 	rn0 = SDL_randf();
	// 	rn1 = SDL_randf();
	// 	bary_triangle(*app.video.pixel_buffer, *app.video.viewport, 
	// 								 v0, v1, {w*rn0, h*rn1}, 0xFF0000FF);
	// }
	// for (int i = 0; i < 50; i++) {
	// 	rn0 = SDL_randf();
	// 	rn1 = SDL_randf();
	// 	draw_trigon(*app.video.pixel_buffer, *app.video.viewport, 
	// 								 v0, v1, {w*rn0, h*rn1}, 0xFFFF0000, 1);
	// }

	// bary_triangle(*app.video.pixel_buffer, *app.video.viewport, 
	// 		v0, v1, mouse, 0xFFFF0000);
	const auto end = core::Clock::now();
  const auto elapsed = std::chrono::duration_cast<core::ms>(end - start).count();


	const auto start_2 = core::Clock::now();
	// for (int i = 0; i < 50; i++) {
	// 	rn0 = SDL_randf();
	// 	rn1 = SDL_randf();
	// 	draw_trigon(*app.video.pixel_buffer, *app.video.viewport, 
	// 								 v0, v1, {w*rn0, h*rn1}, 0xFFFF0000, 0);
	// }

	draw_trigon(*app.video.pixel_buffer, *app.video.viewport, 
							v0, v1, mouse, 0x550000FF, 0);


	// int ww;
	// int hw;
	// SDL_GetWindowSizeInPixels(app.window, &ww, &hw);
	// fmt::print("w {}, h {}\n", ww, hw);

	const auto end_2 = core::Clock::now();
  const auto elapsed_2 = std::chrono::duration_cast<core::ms>(end_2 - start_2).count();
	fmt::print("time bary: \n{}\ntime my\n{}\n", elapsed, elapsed_2);

	// render::draw_rect(*app.video.pixel_buffer, *app.video.viewport, {0.0, 0.0}, {w,h}, color);

	// draw_trigon(*app.video.pixel_buffer, *app.video.viewport, {100.0, 100.0},
	//              {200.0, 200.0}, mouse, 0xFF0000FF);
	
	// static float p1_x = 100;
	// static float p1_y = 100;
	//
	// static float p2_x = 400;
	// static float p2_y = 400;
	//
	//
	// if (app.state.input.shift.down()) {
	// 	sdl_color = {255, 255, 255, 255};
	// 	p1_x  += 1.0;
	// 	p2_x  += 1.0;
	// }
	//
	// if (app.state.input.ctrl.down()) {
	// 	sdl_color = {255, 255, 255, 255};
	// 	p1_x -= 1.0;
	// 	p2_x -= 1.0;
	// }

	// Vec2 p1 {p1_x, app.state.input.mouse.y};
	// Vec2 p2 {p2_x, p2_y};
	// render::draw_rect(*app.video.pixel_buffer, *app.video.viewport, p1, p2, color);
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
