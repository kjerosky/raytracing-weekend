#include <SDL3/SDL.h>
#include <memory>

#include "rt_weekend.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"

float hit_sphere(const glm::vec3& center, float radius, const Ray& r) {
    glm::vec3 oc = center - r.get_origin();
    float a = glm::dot(r.get_direction(), r.get_direction());
    float h = glm::dot(r.get_direction(), oc);
    float c = glm::dot(oc, oc) - radius * radius;
    float discriminant = h * h - a * c;

    if (discriminant < 0.0f) {
        return -1.0f;
    } else {
        return (h - glm::sqrt(discriminant)) / a;
    }
}

// --------------------------------------------------------------------------

glm::vec3 ray_color(Ray& r, const Hittable& world) {
    HitRecord hit_record;
    if (world.hit(r, Interval(0.0f, INF), hit_record)) {
        return 0.5f * (hit_record.normal + glm::vec3(1.0f));
    }

    glm::vec3 unit_direction = glm::normalize(r.get_direction());
    float a = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
}

// --------------------------------------------------------------------------

int main(int argc, char** argv) {
    HittableList world;
    world.add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
    world.add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f));

    float ideal_aspect_ratio = 16.0f / 9.0f;
    int image_width = 400;
    int image_height = static_cast<int>(image_width / ideal_aspect_ratio);
    image_height = glm::max(1, image_height);

    float focal_length = 1.0f;
    float viewport_height = 2.0f;
    float viewport_width = viewport_height * image_width / image_height;
    glm::vec3 camera_center = glm::vec3(0.0f);

    glm::vec3 viewport_u = glm::vec3(viewport_width, 0.0f, 0.0f);
    glm::vec3 viewport_v = glm::vec3(0.0f, -viewport_height, 0.0f);

    glm::vec3 pixel_delta_u = viewport_u / float(image_width);
    glm::vec3 pixel_delta_v = viewport_v / float(image_height);

    glm::vec3 viewport_upper_left = camera_center - glm::vec3(0.0f, 0.0f, focal_length) - viewport_u / 2.0f - viewport_v / 2.0f;
    glm::vec3 pixel_00_location = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

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

    Uint32* scene_pixels;
    int scene_pixels_row_length;
    SDL_LockTexture(scene_texture, nullptr, (void**)&scene_pixels, &scene_pixels_row_length);

    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            glm::vec3 pixel_center = pixel_00_location + (static_cast<float>(x) * pixel_delta_u) + (static_cast<float>(y) * pixel_delta_v);
            glm::vec3 ray_direction = pixel_center - camera_center;
            Ray r(camera_center, ray_direction);

            glm::vec3 pixel_color = ray_color(r, world);
            pixel_color *= 255.0f;

            Uint32 pixel = SDL_MapRGBA(pixel_format_details, nullptr, pixel_color.r, pixel_color.g, pixel_color.b, 255);
            scene_pixels[y * image_width + x] = pixel;
        }
    }

    SDL_UnlockTexture(scene_texture);

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
