#version 150

in vec4 vPos0;
in vec4 vPos1;
in vec3 vNormal;
in vec2 vTex;

out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec3 pos_eye, normal_eye;
out vec2 texCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Project;

uniform float time;

void main() {
     vec4 LightPosition = View * vec4(0.0, 0.0, 10000.0, 1.0);
     vec4 P = vPos0 + (vPos1 - vPos0) * time;
     pos_eye = vec3(View * Model * P);
     normal_eye = vec3(View * Model * vec4(vNormal, 0.0));
     texCoord = vTex;

     fN = normal_eye;
     fE = -pos_eye.xyz;
     fL = LightPosition.xyz;

     if (LightPosition.w != 0.0) {
     	fL = LightPosition.xyz - pos_eye;
     }

     gl_Position = Project * vec4(pos_eye, 1.0);
}
