// simple HelloGLSL fragment shader code (core profile)

#version 400

uniform vec3 drawColor; // default draw color 
out vec4 fragColor;     // fragment’s final color output

void main()
{
    fragColor = vec4(drawColor, 1.0); // Farbe setzen
}
