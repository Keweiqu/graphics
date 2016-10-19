#version 150

in vec4 vPos;
in vec4 vColor;
flat out vec4 color;
uniform mat4 M;
void main() {
    gl_Position = M * vPos;
    color = vColor;
}
