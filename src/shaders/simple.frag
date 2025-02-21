#version 330

in vec3 color;

uniform float t_elapsed;
uniform vec2 window_dimensions;

out vec4 fragment_colour;

const int MAX_ITERATIONS = 40;
const float THRESHOLD = 0.01;
const float MAX_DISTANCE = 10.0;
const vec3 sphere_position = vec3(-0.5, 0.0, 0.0);
const float floor_level = -2.0;

// BLINN-PHONG constants
const vec3 light_position = vec3(3.0, 3.0, -4.0);
const vec3 light_color = vec3(1.0, 1.0, 1.0);
const vec3 ambient_color = vec3(0.1, 0.1, 0.1);
const vec3 object_color = vec3(0.0, 0.5, 0.0);
const float specular_strength = 0.5;
const float shininess = 32.0;

// Rodrigues formula for rotation
vec3 rotate_arbitrary_axis(vec3 p, vec3 axis, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    float dot_prod = dot(axis, p);
    vec3 cross_prod = cross(axis, p);
    return p * c + cross_prod * s + axis * (dot_prod * (1.0 - c));
}

float box_sdf(vec3 point, vec3 dimensions)
{
    vec3 axis = vec3(1.0);
    vec3 rotated = rotate_arbitrary_axis(point,
                                         normalize(axis),
                                         t_elapsed);
    vec3 q = abs(rotated) - dimensions;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sphere_sdf(vec3 point, float radius) {
    const float amplitude = 2.0;
    const float frequency = 1.0;
    const float phase = 0.0;

    vec3 time_offset = vec3(
        sin(frequency * t_elapsed) * amplitude + phase,
        0.0,
        0.0
    );

    return length(point - sphere_position - time_offset) - radius;
}

float get_distance(vec3 point) {
    float dist = sphere_sdf(point, 0.5);
    dist = min(dist, box_sdf(point, vec3(0.3)));
    return dist;
}

vec3 get_normal(vec3 point) {
    float epsilon = 0.001;
    return normalize(vec3(
        get_distance(point + vec3(epsilon, 0, 0)) - get_distance(point - vec3(epsilon, 0, 0)),
        get_distance(point + vec3(0, epsilon, 0)) - get_distance(point - vec3(0, epsilon, 0)),
        get_distance(point + vec3(0, 0, epsilon)) - get_distance(point - vec3(0, 0, epsilon))
    ));
}

void main()
{
    vec2 xy_clip = ((gl_FragCoord.xy * 2 - window_dimensions) / window_dimensions.y) * 2;
    vec3 ray_direction = vec3(0., 0., 1.);
    vec3 ray_origin = vec3(xy_clip, -2);

    float total_distance = 0.;
    vec3 colour = vec3(0.);

    for(int i = 0; i < MAX_ITERATIONS; ++i) {
        vec3 current_position = ray_origin + ray_direction * total_distance;
        float distance = get_distance(current_position);
        total_distance += distance;

        if (total_distance > MAX_DISTANCE) {
            colour = vec3(0.0);
            break;
        }

        if (distance < THRESHOLD) {
            vec3 normal = get_normal(current_position);
            vec3 light_dir = normalize(light_position - current_position);
            vec3 view_dir = normalize(-current_position);
            vec3 halfway_dir = normalize(light_dir + view_dir);

            float diff = max(dot(normal, light_dir), 0.0);
            float spec = pow(max(dot(normal, halfway_dir), 0.0), shininess);

            vec3 ambient = ambient_color;
            vec3 diffuse = diff * light_color;
            vec3 specular = specular_strength * spec * light_color;

            colour = object_color * (ambient + diffuse) + specular;
            break;
        }
    }

    fragment_colour = vec4(colour, 1.0);
}
