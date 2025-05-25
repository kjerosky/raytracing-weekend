#include "Camera.h"

Camera::Camera(int image_width, float desired_aspect_ratio)
:
image_width(image_width),
desired_aspect_ratio(desired_aspect_ratio) {

    initialize();
}

// --------------------------------------------------------------------------

Camera::~Camera() {
    // nothing to do for now
}

// --------------------------------------------------------------------------

void Camera::initialize() {
    image_height = static_cast<int>(image_width / desired_aspect_ratio);
    image_height = glm::max(1, image_height);

    center = glm::vec3(0.0f);

    // Determine viewport dimensions.
    float focal_length = 1.0f;
    float viewport_height = 2.0f;
    float viewport_width = viewport_height * image_width / image_height;

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    glm::vec3 viewport_u = glm::vec3(viewport_width, 0.0f, 0.0f);
    glm::vec3 viewport_v = glm::vec3(0.0f, -viewport_height, 0.0f);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / float(image_width);
    pixel_delta_v = viewport_v / float(image_height);

    // Calculate the location of the upper-left pixel.
    glm::vec3 viewport_upper_left = center - glm::vec3(0.0f, 0.0f, focal_length) - viewport_u / 2.0f - viewport_v / 2.0f;
    pixel_00_location = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
}

// --------------------------------------------------------------------------

glm::vec3 Camera::ray_color(const Ray& r, const Hittable& world) const {
    HitRecord hit_record;
    if (world.hit(r, Interval(0.0f, INF), hit_record)) {
        return 0.5f * (hit_record.normal + glm::vec3(1.0f));
    }

    // Default to a background sky gradient.
    glm::vec3 unit_direction = glm::normalize(r.get_direction());
    float a = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
}

// --------------------------------------------------------------------------

void Camera::render_to_texture(const Hittable& world, SDL_Texture* texture, const SDL_PixelFormatDetails* pixel_format_details) {
    Uint32* scene_pixels;
    int scene_pixels_row_length;
    SDL_LockTexture(texture, nullptr, (void**)&scene_pixels, &scene_pixels_row_length);

    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            glm::vec3 pixel_center = pixel_00_location + (static_cast<float>(x) * pixel_delta_u) + (static_cast<float>(y) * pixel_delta_v);
            glm::vec3 ray_direction = pixel_center - center;
            Ray r(center, ray_direction);

            glm::vec3 pixel_color = ray_color(r, world);
            pixel_color *= 255.0f;
            Uint32 byte_remapped_pixel_color = SDL_MapRGBA(pixel_format_details, nullptr, pixel_color.r, pixel_color.g, pixel_color.b, 255);

            scene_pixels[y * image_width + x] = byte_remapped_pixel_color;
        }
    }

    SDL_UnlockTexture(texture);
}

// --------------------------------------------------------------------------

int Camera::get_image_width() {
    return image_width;
}

// --------------------------------------------------------------------------

int Camera::get_image_height() {
    return image_height;
}
