// switch vertex shader

#version 400 core


vec4 doGouraudShading(in mat4 matModelView, in vec3 vecPosition, in vec3 vecNormal);
// TODO: define doPhongShading() vertex shader function for the phong shading renderer
void doPhongShading(in mat4 matModelView, in vec3 vecPosition, in vec3 vecNormal, out vec3 vsVertex, out vec3 vsNormal);

// TODO: define switch flag uniform to switch between renderers
uniform bool uSwitchPhongShader;

uniform mat4 matModelView;
uniform mat4 matProjection;

layout(location = 0) in vec3 vecPosition;
layout(location = 1) in vec3 vecNormal;

out vec4 vecSurfaceColor;
//flat out vec4 vecSurfaceColor;

// TODO: define out variables you need in the phong shader
out vec3 vsVertex;
out vec3 vsNormal;


void main(void)
{
    // TODO: add switch flag to switch between renderers
    if (!uSwitchPhongShader)
    {
        vecSurfaceColor = doGouraudShading(matModelView, vecPosition, vecNormal);
    }
    else
    {
        doPhongShading(matModelView, vecPosition, vecNormal, vsVertex, vsNormal);
    }

    // set final vertex position
    gl_Position = matProjection * matModelView * vec4(vecPosition, 1.0);
}
