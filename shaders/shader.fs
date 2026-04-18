#version 330 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 TextureCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
   FragColor = mix(texture(texture0, TextureCoord), texture(texture1, TextureCoord), 0.5);
};
