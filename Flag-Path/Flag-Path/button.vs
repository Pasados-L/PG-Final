#version 330 core
layout(location = 0) in vec2 aPos;

uniform vec2 uPosition;
uniform vec2 uSize;

void main() {
    vec2 scaledPos = aPos * uSize + uPosition;
    gl_Position = vec4(scaledPos * 2.0 - 1.0, 0.0, 1.0);  // Normalized screen coords
}
