#include "app.hpp"

bool app_init(App &app, int width, int height) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return false;
  }
  SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE |
                                 SDL_WINDOW_HIGH_PIXEL_DENSITY |
                                 SDL_WINDOW_MOUSE_CAPTURE;

  app.window = SDL_CreateWindow("main", width, height, window_flags);
  if (!app.window) {
    return false;
  }
  app.renderer = SDL_CreateRenderer(app.window, NULL);
  if (!app.renderer) {
    return false;
  }
  SDL_SetRenderVSync(app.renderer, 1);
  SDL_SetWindowPosition(app.window, SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED);

  app.width = width;
  app.height = height;
	app.pixel_density = 1;

  app.window_texture =
      SDL_CreateTexture(app.renderer, SDL_PIXELFORMAT_RGBA32,
                        SDL_TEXTUREACCESS_TARGET, width, height);
  return true;
}


void process_events(State& state) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
       state.context.quit = true;
      break;

		default: break;
		}
	}
}


void query_input(State& state) {
	SDL_MouseButtonFlags mouse_flags =
		SDL_GetMouseState(&state.input.mouse.x, &state.input.mouse.y);
	state.input.mouse_left.down(mouse_flags & SDL_BUTTON_LMASK);
	state.input.mouse_mid.down(mouse_flags & SDL_BUTTON_MMASK);
	state.input.mouse_right.down(mouse_flags & SDL_BUTTON_RMASK);

	const bool* key_states = SDL_GetKeyboardState(nullptr);
	state.input.shift.down(key_states[SDL_SCANCODE_LSHIFT]);
	state.input.ctrl.down(key_states[SDL_SCANCODE_LCTRL]);
}
