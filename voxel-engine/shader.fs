#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D u_texture;
uniform vec3 u_color;

void main() {
//    FragColor = texture(u_texture, TexCoord);
    FragColor = vec4(u_color, 1.0f);
}
