// simple toonify rendering fragment shader code (core profile)

#version 400


vec4 toonify(in float intensity);

in vec3 lightDir;
in vec3 normal; 
out vec4 fragColor; // fragment’s final color (default location)
//layout(location = 0) out vec4 fragColor;



void main()
{
    float intensity;
    vec3 n;

    n = normalize(normal);
    intensity = max(dot(lightDir,n), 0.0); // schaut das die Lichtintensität nicht kleiner 0 ist, da unphysikalisch

    fragColor = toonify(intensity);
}