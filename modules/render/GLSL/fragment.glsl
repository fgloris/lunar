R"(
#version 430 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
    // 如果想要混合颜色和纹理，可以使用：
    // FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
}
)"