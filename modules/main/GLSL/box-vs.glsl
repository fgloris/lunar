R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    normal = normalize(normalMatrix * aNormal);
    fragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
}
)"