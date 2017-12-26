#version 330 core

uniform sampler1D colors_1d;
uniform int max_iter;
uniform int threshold;
uniform vec2 center_pos;
uniform float scale;
in vec2 pos;
out vec4 color;

void main() {
    vec2 pos_norm = pos * exp(scale) + center_pos;
    pos_norm = vec2((pos_norm.x + 1) / 2, (pos_norm.y + 1) / 2);
    float x0 = pos_norm.x * 3.5 - 2.5;
    float y0 = pos_norm.y * 2 - 1;
    int i;
    float x = 0, y = 0, new_x;
    for (i = 0; i < max_iter; i++) {
        new_x = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = new_x;
        if (x * x + y * y > threshold) {
            break;
        }
    }

    float col = i;
    if (i < max_iter) {
        col = col + 1 - log((log(x * x + y * y) / 2) / log(2.0)) / log(2.0);
    }

    color = texture(colors_1d, col / max_iter, 0);
}