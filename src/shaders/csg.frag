#version 330 core

uniform float t_elapsed;
uniform vec2 window_dimensions;

out vec4 FragColour;

const int MAX_ITERATIONS = 300;
const float THRESHOLD = 0.001;
const float MAX_DISTANCE = 10.0;
const float floor_level = -2.0;

// BLINN-PHONG constants
const vec3 light_position = vec3(0.0, 3.0, -1.0);
const vec3 light_color = vec3(1.0, 1.0, 1.0);
const vec3 ambient_color = vec3(0.3, 0.3, 0.3);
const vec3 object_color = vec3(0.3, 0.7, 0.3);
const float specular_strength = 0.5;
const float shininess = 32.0;

// Additional lighting parameters
const float attenuation_factor = 0.05;
const vec3 rim_color = vec3(0.1, 0.3, 0.1);
const float rim_power = 3.0;
const float rim_intensity = 0.3;

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
    vec3 q = abs(point) - dimensions;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sphere_sdf(vec3 point, float radius)
{
    return length(point) - radius;
}

float get_distance(vec3 point) {
    vec3 time_offset = vec3(
        sin(1.0 * t_elapsed + 0.0) * 1.3,
        0.0,
        0.0
    );
    float dist = sphere_sdf(point - time_offset, 0.7);

    vec3 axis = vec3(1.0);
    vec3 rotated = rotate_arbitrary_axis(point,
                                         normalize(axis),
                                         t_elapsed);
    dist = min(dist, box_sdf(rotated, vec3(0.3)));
    dist = min(dist, box_sdf(point, vec3(2.0, 0.3, 0.3)));

    dist = min(dist, box_sdf(point - vec3(0.0, 2.0, 0.0),
                             vec3(0.3, 0.3, 2.0)
                             )
               );
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

// Improved shadow calculation with soft shadows
float calculate_soft_shadow(vec3 point, vec3 light_pos, float k) {
    vec3 light_dir = normalize(light_pos - point);
    float distance_to_light = length(light_pos - point);
    float shadow_distance = 0.01;
    float result = 1.0;
    vec3 new_point = point + (get_normal(point) * THRESHOLD * 2.0);

    for(int i = 0; i < 64; ++i) {
        if(shadow_distance >= distance_to_light) break;

        vec3 shadow_position = new_point + light_dir * shadow_distance;
        float df = get_distance(shadow_position);

        if(df < THRESHOLD) return 0.1;

        // Soft shadow calculation
        result = min(result, k * df / shadow_distance);
        shadow_distance += df;
    }

    return clamp(result, 0.1, 1.0);
}

// Calculate light attenuation
float calculate_attenuation(vec3 point, vec3 light_pos) {
    float distance = length(light_pos - point);
    return 1.0 / (1.0 + attenuation_factor * distance * distance);
}

void main()
{
    vec2 xy_clip = ((gl_FragCoord.xy * 2 - window_dimensions) / window_dimensions.y) * 1.0;
    vec2 camera_pitch = vec2(0.0, -0.7);
    vec3 ray_direction = normalize(vec3(xy_clip + camera_pitch, 1.0));
    vec3 ray_origin = vec3(0.0, 3.0, -4.0);

    float total_distance = 0.;
    vec3 colour = vec3(0.);

    for(int i = 0; i < MAX_ITERATIONS; ++i) {
        vec3 current_position = ray_origin + ray_direction * total_distance;
        current_position = rotate_arbitrary_axis(current_position,
                                                 normalize(vec3(0.,1.,0.)),
                                                 t_elapsed * 0.8);
        float distance = get_distance(current_position);
        total_distance += distance;

        if (total_distance > MAX_DISTANCE) {
            // Create a simple gradient background
            float t = 0.5 * (ray_direction.y + 1.0);
            colour = mix(vec3(0.5, 0.7, 1.0), vec3(0.1, 0.1, 0.3), t);
            break;
        }

        if (distance < THRESHOLD) {
            // IMPROVED BLINN-PHONG SHADING
            vec3 normal = get_normal(current_position);
            vec3 light_dir = normalize(light_position - current_position);
            vec3 view_dir = normalize(ray_origin - current_position);
            vec3 halfway_dir = normalize(light_dir + view_dir);

            // Basic lighting components
            float diff = max(dot(normal, light_dir), 0.0);
            float spec = pow(max(dot(normal, halfway_dir), 0.0), shininess);

            // Calculate attenuation
            float attenuation = calculate_attenuation(current_position, light_position);

            // Calculate soft shadows
            float shadow = calculate_soft_shadow(current_position, light_position, 16.0);

            // Calculate rim lighting (emphasizes edges)
            float rim = 1.0 - max(dot(normal, view_dir), 0.0);
            rim = pow(rim, rim_power) * rim_intensity;

            // Combine all lighting components
            vec3 ambient = ambient_color * object_color;
            vec3 diffuse = diff * light_color * object_color * attenuation;
            vec3 specular = specular_strength * spec * light_color * attenuation;
            vec3 rim_light = rim * rim_color;

            // Apply shadow to diffuse and specular components
            colour = ambient + (diffuse + specular) * shadow + rim_light;

            // Add subtle color variations based on normals
            colour += normal * 0.05;

            // Apply gamma correction
            colour = pow(colour, vec3(1.0/2.2));

            break;
        }
    }

    FragColour = vec4(colour, 1.0);
}
