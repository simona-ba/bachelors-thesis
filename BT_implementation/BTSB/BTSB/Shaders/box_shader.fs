#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = vec4(0.0,1.0,0.0,1.0) * (gl_FragCoord.x / 1000);
}