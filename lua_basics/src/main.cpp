#include "core.hpp"
#include "graphics.hpp"
#include "render.hpp"
#include "app.hpp"

#include "foo.hpp"


int main() {
  try {
		// maybe do the init in the constructor?
		// also maybe include the custom pixel buffer?
		App app;
		if (!app_init(app, 640, 480)) { return 1; }

    // render::PixelBuffer window_buffer{app.width * app.pixel_density,
																			// app.height * app.pixel_density};
    // window_buffer.clear(0x00000000);

		// bar_func();

    while (!app.state.context.quit) {
			process_events(app.state);
			query_input(app.state);
			update_viewport(app.state, *app.video.viewport);

      SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
      SDL_RenderClear(app.renderer);

			// ---- custom graphics
      foo_func(app);
			
      SDL_UpdateTexture(app.window_texture, nullptr, app.video.pixel_buffer->pixels,
                        app.width * 4);
      SDL_RenderTexture(app.renderer, app.window_texture, NULL, NULL);

			// ---- sdl present renderer
      SDL_RenderPresent(app.renderer);
    }

  } catch (const std::exception &e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
