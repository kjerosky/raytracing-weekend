#include "rt_weekend.h"

#include <memory>
#include <fstream>

#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"

void write_texture_to_file(SDL_Texture* texture, const SDL_PixelFormatDetails* pixel_format_details, int image_width, int image_height) {
    std::ofstream file("output.ppm");
    file << "P3\n";
    file << image_width << " " << image_height << "\n";
    file << "255\n";

    Uint32* pixels;
    int pixels_row_length;
    SDL_LockTexture(texture, nullptr, (void**)&pixels, &pixels_row_length);

    for (int i = 0; i < image_width * image_height; i++) {
        Uint32 pixel_color = pixels[i];

        Uint8 r, g, b, a;
        SDL_GetRGBA(pixel_color, pixel_format_details, nullptr, &r, &g, &b, &a);

        file << static_cast<int>(r) << " " << static_cast<int>(g) << " " << static_cast<int>(b) << "\n";
    }

    SDL_UnlockTexture(texture);

    file.close();
}

// --------------------------------------------------------------------------

int main(int argc, char** argv) {
    const int REQUESTED_IMAGE_WIDTH = 400;
    const float DESIRED_ASPECT_RATIO = 16.0f / 9.0f;
    const int SAMPLES_PER_PIXEL = 100;

    HittableList world;
    world.add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
    world.add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f));

    Camera camera(REQUESTED_IMAGE_WIDTH, DESIRED_ASPECT_RATIO, SAMPLES_PER_PIXEL);

    int image_width = camera.get_image_width();
    int image_height = camera.get_image_height();

    // --- sdl setup ---

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "[ERROR] Could not initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Ray Tracing in One Weekend", image_width, image_height, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "[ERROR] Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "[ERROR] Could not create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return 1;
    }

    SDL_PixelFormat pixel_format = SDL_PIXELFORMAT_RGBA8888;
    SDL_Texture* scene_texture = SDL_CreateTexture(renderer, pixel_format, SDL_TEXTUREACCESS_STREAMING, image_width, image_height);
    if (!scene_texture) {
        std::cerr << "[ERROR] Could not create scene texture: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }

    const SDL_PixelFormatDetails* pixel_format_details = SDL_GetPixelFormatDetails(pixel_format);
    if (!pixel_format_details) {
        std::cerr << "[ERROR] Could not create pixel format details: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(scene_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }

    // --- render ---

    Uint64 render_start_timestamp = SDL_GetTicks();
    camera.render_to_texture(world, scene_texture, pixel_format_details);
    Uint64 render_end_timestamp = SDL_GetTicks();

    std::cout << "Render time (seconds): " << (render_end_timestamp - render_start_timestamp) / 1000.0f << std::endl;

    write_texture_to_file(scene_texture, pixel_format_details, image_width, image_height);

    bool is_running = true;
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                is_running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                is_running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, scene_texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(scene_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
