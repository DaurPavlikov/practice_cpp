#version 460
in vec2 uv;
out vec4 fragment_color;

uniform sampler2D texture_0;

void main(){
    fragment_color = texture(texture_0, uv);
}