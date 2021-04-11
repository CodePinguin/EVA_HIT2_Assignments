// gouraud shading fragment shader
// (for one light only and without emissive component)

#version 400


in vec4 vecSurfaceColor;
//flat in vec4 vecSurfaceColor;

vec4 doGouraudShading(void)
{
    return vecSurfaceColor;
}