R"(
#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

const float threshold = 0.04; // 深度差值阈值，可以根据需要调整
const float offset = 1.0 / 800.0; // 像素偏移量，需要根据实际分辨率调整

float gaussian(float x, float sigma) {
    return exp(-0.5 * (x * x) / (sigma * sigma)) / (sigma * sqrt(2.0 * 3.14159265359));
}

bool checkDepthDiscontinuity() {
    float centerDepth = texture(depthTexture, TexCoords).r;
    
    // 5x5核的遍历
    for(int x = -2; x <= 2; x++) {
        for(int y = -2; y <= 2; y++) {
            // 跳过中心像素
            if(x == 0 && y == 0) continue;
            
            // 计算采样位置
            vec2 samplePos = TexCoords + vec2(x * offset, y * offset);
            float sampleDepth = texture(depthTexture, samplePos).r;
            
            // 检查深度差值
            if(sampleDepth - centerDepth > threshold) {
                return true; // 发现深度不连续
            }
        }
    }
    return false; // 没有发现深度不连续
}

void blur() {
    float depth = texture(depthTexture, TexCoords).r;
    float blurSize = 5 * depth;

    vec3 color = vec3(0.0);
    float totalWeight = 0.0;
    for(int x = -int(blurSize); x <= int(blurSize); x++) {
        for(int y = -int(blurSize); y <= int(blurSize); y++) {
            vec2 offset = vec2(x, y) / textureSize(screenTexture, 0);
            float weight = gaussian(length(offset), blurSize / 2.0);
            color += texture(screenTexture, TexCoords + offset).rgb * weight;
            totalWeight += weight;
        }
    }
    color /= totalWeight;
    
    FragColor = vec4(color, 1.0);

}

void main()
{
    //vec3 color = texture(screenTexture, TexCoords).rgb;
    
    // 如果检测到深度不连续，设置为黑色
    if(checkDepthDiscontinuity()) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        // 否则使用原始颜色
        blur();
        //FragColor = vec4(color, 1.0);
    }
}
)"