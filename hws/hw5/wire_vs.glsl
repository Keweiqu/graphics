#version 150

in vec3 vPos;
in vec3 vNormal;
out vec4 color;

uniform mat4 ModelView;
uniform mat4 Project;
void main() {
     gl_Position = Project * ModelView * vec4(vPos, 1.0);	    
     color = vec4(0.0, 0.0, 0.0, 1.0);
}