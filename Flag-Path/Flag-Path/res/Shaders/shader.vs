#version 330 core

layout(location = 0) in vec3 aPos;           // posición base
layout(location = 1) in vec3 aNormal;        // normal base
layout(location = 2) in vec2 aTexCoords;     // uv

layout(location = 3) in ivec4 BoneIDs;
layout(location = 4) in vec4 Weights;
layout(location = 5) in vec3 PosMorph1;
layout(location = 6) in vec3 PosMorph2;

uniform float morphWeight1;
uniform float morphWeight2;
// ... más pesos

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
    // Interpolación simple de morph targets
    vec3 morphedPos = aPos 
                     + morphWeight1 * (PosMorph1 - aPos)
                     + morphWeight2 * (PosMorph2 - aPos);
    // Puedes hacer lo mismo con normales si los tienes

    gl_Position = projection * view * model * vec4(morphedPos, 1.0);
    TexCoords = aTexCoords;
}
