#ifndef CAMERA_H
#define CAMERA_H

#include "rt_weekend.h"

#include "Hittable.h"

class Camera {

public:

    Camera(int image_width, float desired_aspect_ratio);
    ~Camera();

    void render_to_texture(const Hittable& world, SDL_Texture* texture, const SDL_PixelFormatDetails* pixel_format_details);
    int get_image_width();
    int get_image_height();

private:

    int image_width;
    float desired_aspect_ratio;

    int image_height;

    glm::vec3 center;
    glm::vec3 pixel_00_location;
    glm::vec3 pixel_delta_u;
    glm::vec3 pixel_delta_v;

    void initialize();
    glm::vec3 ray_color(const Ray& r, const Hittable& world) const;
};

#endif
