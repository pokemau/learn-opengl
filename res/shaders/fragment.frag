#version 330 core

out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D tex1;

void main() {
    fragColor = texture(tex1, texCoord);
}
