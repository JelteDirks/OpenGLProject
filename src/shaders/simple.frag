#version 330

in vec3 color;

uniform vec2 window_dimensions;

out vec4 fragment_colour;

void main()
{
    vec2 xy_clip = (gl_FragCoord.xy * 2 - window_dimensions) / window_dimensions.y;

    fragment_colour = vec4(abs(xy_clip), 0.0, 1.0);
}
