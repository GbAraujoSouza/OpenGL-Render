#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TextureCoord;
uniform mat4 transformMatrix;

void main() {
	gl_Position = vec4(aPos, 1.0f) * transformMatrix;
	vertexColor = vec4(aColor.x, aColor.y, aColor.z, 1.0f);
	TextureCoord = aTexCoord;
};
