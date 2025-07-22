#version 330 core
out vec4 FragColor;
uniform sampler2D sunTexture;
in vec2 TexCoords;
void main()
{
    vec3 texColor = texture(sunTexture, TexCoords).rgb;
    FragColor = vec4(texColor, 1.0);
}