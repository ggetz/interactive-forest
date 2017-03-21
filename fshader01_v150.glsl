#version 150

in vec4 vColor;
in vec2 texCoord;

uniform sampler2D textureID;

out vec4 fragColor;

void main() 
{ 
    fragColor = texture(textureID,texCoord) * vColor;
} 