// gouraud shading fragment shader
// (for one light only and without emissive component)

#version 400


in vec4 vecSurfaceColor;
//flat in vec4 vecSurfaceColor;

layout (location = 0) out vec4 fragColor; // fragment’s final color (default location)


void main(void)
{
    fragColor = vecSurfaceColor;
}