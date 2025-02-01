R"(
vec3 RGB2HSV(vec3 rgb) {
    float max = max(rgb.r, max(rgb.g, rgb.b));
    float min = min(rgb.r, min(rgb.g, rgb.b));
    float delta = max - min;
    
    vec3 hsv = vec3(0.0, 0.0, max);  // v = max
    
    if (max != 0.0) {  // s = delta/max
        hsv.y = delta / max;
    }
    
    if (delta != 0.0) {
        if (max == rgb.r) {
            hsv.x = (rgb.g - rgb.b) / delta;  // h
            if (rgb.g < rgb.b) 
                hsv.x += 6.0;
        } else if (max == rgb.g) {
            hsv.x = 2.0 + (rgb.b - rgb.r) / delta;
        } else {
            hsv.x = 4.0 + (rgb.r - rgb.g) / delta;
        }
        hsv.x /= 6.0;
    }
    
    return hsv;
}

vec3 HSV2RGB(vec3 hsv) {
    if (hsv.y == 0.0) {
        return vec3(hsv.z);
    }
    
    float h = hsv.x * 6.0;
    float i = floor(h);
    float f = h - i;
    float p = hsv.z * (1.0 - hsv.y);
    float q = hsv.z * (1.0 - hsv.y * f);
    float t = hsv.z * (1.0 - hsv.y * (1.0 - f));
    
    if (i == 0.0) return vec3(hsv.z, t, p);
    else if (i == 1.0) return vec3(q, hsv.z, p);
    else if (i == 2.0) return vec3(p, hsv.z, t);
    else if (i == 3.0) return vec3(p, q, hsv.z);
    else if (i == 4.0) return vec3(t, p, hsv.z);
    else return vec3(hsv.z, p, q);
}

vec3 color_thinning(vec3 rgb, float threshold) {
    vec3 hsv = RGB2HSV(rgb);
    float value = hsv.z;
    value = float(int((value * threshold) + 0.5)) / threshold;
    return HSV2RGB(vec3(hsv.x, hsv.y, value));
}
)"