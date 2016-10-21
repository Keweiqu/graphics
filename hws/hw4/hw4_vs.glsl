#version 150

in vec4 vPos0;
in vec4 vPos1;
in vec4 vColor;
flat out vec4 color;
uniform mat4 M;
uniform float time;
void main() {
  vec4 P = vPos0 + (vPos1 - vPos0) * time;
    gl_Position = M * P;
    color = vColor;
}
