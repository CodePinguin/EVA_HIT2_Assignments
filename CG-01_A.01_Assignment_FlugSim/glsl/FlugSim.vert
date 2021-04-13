// simple modeling vertex shader code (core profile)

#version 400


layout (location = 0) in vec4 vecPosition;
layout (location = 1) in vec4 vecColor;
layout (location = 2) in vec2 vecTexture;

uniform mat4 matModelView;
uniform mat4 matProjection;
uniform mat4 matTexture;

out vec4 color;

void main()
{
    color = vecColor;
    gl_Position = matProjection * matModelView * vecPosition;
}
