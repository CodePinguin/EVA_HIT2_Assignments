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


uniform mat4 matModelView;
uniform mat4 matProjection;
uniform mat4 matTexture;


out vec4 vecSurfaceColor;
out vec4 texCoord;


void main(void)
{
    // calculate eye coordinates of vertex- and normal vectors, because
    // specular reflection is calculated in eye space coordinates
    vec3 position_ec = vec3(matModelView * vec4(vecPosition, 1.0));

    // transform vertex normal orientation into eye coordinates using normal matrix
    mat3 normal_matrix = transpose( inverse( mat3(matModelView) ) );
    vec3 N = normalize( normal_matrix * vecNormal );

    // calculate vector from vertex to point light source in eye coordinates
    vec3 L = normalize(ubLightSource.position.xyz - position_ec);

    // calculate vector from vertex to view/eye point
    // (we are in eye coordinates, so eye position is (0,0,0))
    vec3 V = normalize(-position_ec);

    // calculate reflection vector using built-in function reflect()
    // (reflect() requires incident direction of point light vector, so reverse direction)
    vec3 R = normalize(reflect(-L, N));

    // calculate ambient term using light and material components
    vec4 Iamb = ubLightSource.ambient * ubMaterial.ambient;

    // calculate diffuse term using light and material components
    vec4 Idiff = ubLightSource.diffuse * ubMaterial.diffuse * max(dot(L, N), 0.0);

    // calculate specular term using light and material components
    vec4 Ispec = ubLightSource.specular * ubMaterial.specular * pow( max(dot(R, V), 0.0), ubMaterial.shininess );

    // summation of total light intensity (ignoring emissive component)
    vec4 Itotal = Iamb + Idiff + Ispec;

    // output total fragment color
    vecSurfaceColor = Itotal;

    // use model texture coordinates and transform with texture matrix
    texCoord = matTexture * vec4(vecTexture, 0.0, 1.0);

    // set final vertex position
    gl_Position = matProjection * matModelView * vec4(vecPosition, 1.0);
}