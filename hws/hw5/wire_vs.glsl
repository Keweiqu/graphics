#version 150

in vec4 vPos;
in vec3 vNormal;
out vec4 color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Project;
void main() {
     gl_Position = Project * View * Model * vPos;
     color = vec4(0.0, 0.0, 0.0, 1.0);
}
