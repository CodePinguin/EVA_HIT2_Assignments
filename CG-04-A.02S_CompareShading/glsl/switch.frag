// switch fragment shader 
// (for one light only and without emissive component)

#version 400 core


vec4 doGouraudShading(void);
// TODO: define doPhongShading() fragment shader function for the phong shading renderer
vec4 doPhongShading(in vec3 Vertex, in vec3 Normal);

// TODO: define switch flag uniform to switch between renderers
uniform bool uSwitchPhongShader;

// TODO: define in variables needed in the phong shader
in vec3 vsVertex;
in vec3 vsNormal;

out vec4 fragColor; // fragment’s final color (default location)

void main(void)  
{
    // TODO: add switch flag to switch between renderers
    if (!uSwitchPhongShader)
    {
        fragColor = doGouraudShading();
    }
    else
    {
        fragColor = doPhongShading(vsVertex, vsNormal);
    }
}