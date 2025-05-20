#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;
uniform float skyboxIntensity;

void main()
{
    vec4 texColor = texture(skybox, TexCoords);
    color = vec4(texColor.rgb * skyboxIntensity, texColor.a);
}
