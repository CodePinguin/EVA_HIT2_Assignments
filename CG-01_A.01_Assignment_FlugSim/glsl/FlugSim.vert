// texturing vertex shader (using gouraud shading)

#version 420


// define uniform block for light properties
layout (std140, binding = 2) uniform LightProperties
{
    vec4 position;      // position of point light in eye space coordinates
    vec4 ambient;       // amount of ambient light emitted
    vec4 diffuse;       // amount of diffuse light emitted
    vec4 specular;      // amount of specular light emitted
} ubLightSource;

// define uniform block for material properties
layout (std140, binding = 4) uniform MaterialProperties
{
    vec4 ambient;       // what part of ambient light is reflected
    vec4 diffuse;       // what part of diffuse light is scattered
    vec4 specular;      // what part of specular light is scattered
    float shininess;    // exponent for sharpening specular reflection
} ubMaterial;


layout (location = 0) in vec3 vecPosition;
layout (location = 1) in vec3 vecNormal;
layout (location = 2) in vec2 vecTexture;
layout (location = 3) in vec4 vecColor;

uniform mat4 matModelView;
uniform mat4 matProjection;
uniform mat4 matTexture;


out vec4 vecSurfaceColor;
out vec4 texCoord;
out vec4 use_texture;


void main(void)
{
    use_texture = vec4(1,0,0,0);

    if (vecColor.x > 0 || vecColor.y > 0 || vecColor.z > 0)
    {
        gl_Position = matProjection * matModelView * vec4(vecPosition, 1.0);
        vecSurfaceColor = vecColor;
        use_texture = vec4(0,0,0,0); //tell frag shader to use color normally
        return;
    }


    // output total fragment color
    vecSurfaceColor = vec4(1,1,1,1); // Itotal;

    // use model texture coordinates and transform with texture matrix
    texCoord = matTexture * vec4(vecTexture, 0.0, 1.0);

    // set final vertex position
    gl_Position = matProjection * matModelView * vec4(vecPosition, 1.0);
}