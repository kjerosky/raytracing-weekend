#include "Camera.h"

Camera::Camera(int image_width, float desired_aspect_ratio, int samples_per_pixel)
:
image_width(image_width),
desired_aspect_ratio(desired_aspect_ratio),
samples_per_pixel(samples_per_pixel) {

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

    pixels_sample_scale = 1.0f / samples_per_pixel;

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

Ray Camera::get_ray(int x, int y) {
    glm::vec3 offset = sample_square();
    glm::vec3 pixel_sample = pixel_00_location + (x + offset.x) * pixel_delta_u + (y + offset.y) * pixel_delta_v;

    glm::vec3 ray_origin = center;
    glm::vec3 ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
}

// --------------------------------------------------------------------------

glm::vec3 Camera::sample_square() const {
    // Returns the vector to a random point in the [-0.5, -0.5] - (+0.5, +0.5) unit square.
    return glm::vec3(random_float() - 0.5f, random_float() - 0.5f, 0.0f);
}

// --------------------------------------------------------------------------

void Camera::render_to_texture(const Hittable& world, SDL_Texture* texture, const SDL_PixelFormatDetails* pixel_format_details) {
    Uint32* scene_pixels;
    int scene_pixels_row_length;
    SDL_LockTexture(texture, nullptr, (void**)&scene_pixels, &scene_pixels_row_length);

    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            glm::vec3 pixel_color = glm::vec3(0.0f);
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                Ray r = get_ray(x, y);
                pixel_color += ray_color(r, world);
            }

            pixel_color *= pixels_sample_scale;

            static const Interval intensity(0.000f, 0.999f);
            Uint8 red = 256 * intensity.clamp(pixel_color.r);
            Uint8 green = 256 * intensity.clamp(pixel_color.g);
            Uint8 blue = 256 * intensity.clamp(pixel_color.b);
            Uint32 byte_mapped_color = SDL_MapRGBA(pixel_format_details, nullptr, red, green, blue, 255);

            scene_pixels[y * image_width + x] = byte_mapped_color;
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
