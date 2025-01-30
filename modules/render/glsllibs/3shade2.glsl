R"(
vec3 RGB2HSV(vec3 rgb) {
    float max = max(rgb.r, max(rgb.g, rgb.b));
    float min = min(rgb.r, min(rgb.g, rgb.b));
    float delta = max - min;
    float hue = 0.0;
    if (delta != 0.0) {
        if (max == rgb.r) hue = (rgb.g - rgb.b) / delta;
        else if (max == rgb.g) hue = (rgb.b - rgb.r) / delta + 2.0;
        else hue = (rgb.r - rgb.g) / delta + 4.0;
    }
    return vec3(hue, delta / max, max);
}

vec3 HSV2RGB(vec3 hsv) {
    float h = hsv.x;
    float s = hsv.y;
    float v = hsv.z;
    float c = v * s;
    float x = c * (1.0 - abs(mod(h * 6.0, 2.0) - 1.0));
    float m = v - c;
    return vec3(m + c, m + x, m);
}

vec3 cartoonify(vec3 rgb) {
    vec3 hsv = RGB2HSV(rgb);
    float value = hsv.z;
    value = float(int(value * 5)) / 5;
    return HSV2RGB(vec3(hsv.x, hsv.y, value));
}

)"