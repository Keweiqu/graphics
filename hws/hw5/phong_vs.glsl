#version 150

in vec4 vPos;
in vec3 vNormal;

out vec3 fN;
out vec3 fE;
out vec3 fL;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Project;

void main() {
     vec4 LightPosition = View * vec4(0.0, 5.0, 0.0, 1.0);
     vec4 pos = View * Model * vPos;

     fN = (View * Model * vec4(vNormal, 0.0)).xyz;
     fE = (View * Model * pos).xyz;
     fL = LightPosition.xyz;

     if( LightPosition.w != 0.0) {
     	 fL = LightPosition.xyz - pos.xyz;
     }
     gl_Position = Project * View * Model * vPos;
}
