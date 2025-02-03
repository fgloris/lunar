R"(
#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    //FragColor = vec4(color_thinning(texture(screenTexture, TexCoords).rgb, 2.0), 1.0);
    FragColor = vec4(texture(screenTexture, TexCoords));
}
)"