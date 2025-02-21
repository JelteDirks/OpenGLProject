#version 330

in vec3 color;

uniform vec2 window_dimensions;

out vec4 fragment_colour;

const int MAX_ITERATIONS = 50;
const float THRESHOLD = 0.01;
const float MAX_DISTANCE = 10.0;
const vec3 sphere_position = vec3(0.);

float sphere_sdf(vec3 point, float radius) {
    return length(point - sphere_position) - radius;
}

float get_distance(vec3 point) {
    return sphere_sdf(point, 0.5);
}

void main()
{
    vec2 xy_clip = (gl_FragCoord.xy * 2 - window_dimensions) / window_dimensions.y;
    vec3 ray_direction = vec3(0., 0., 1.);

    float total_distance = 0.;
    vec3 colour = vec3(0.);

    for(int i = 0; i < MAX_ITERATIONS; ++i) {
        vec3 ray_origin = vec3(xy_clip, -2);
        float distance = get_distance(ray_origin + ray_direction * total_distance);
        total_distance += distance;

        if (total_distance > MAX_DISTANCE) {
            colour = vec3(0.0);
        }

        if (distance < THRESHOLD) {
            colour = vec3(0.0, 0.5, 0.0);
        }
    }


    fragment_colour = vec4(colour, 1.0);
}
