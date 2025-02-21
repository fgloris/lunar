R"(
#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

const float threshold = 0.04; // 深度差值阈值，可以根据需要调整
const float offset = 1.0 / 800.0; // 像素偏移量，需要根据实际分辨率调整

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

void main()
{
    vec3 color = texture(screenTexture, TexCoords).rgb;
    
    // 如果检测到深度不连续，设置为黑色
    if(checkDepthDiscontinuity()) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        // 否则使用原始颜色
        FragColor = vec4(color, 1.0);
    }
}
)"