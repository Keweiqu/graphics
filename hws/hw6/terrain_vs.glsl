#version 150

in vec4 vPos;
in vec3 vNormal;
in vec2 vTex;

out vec4 pos;
out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec2 texCoord;
out vec3 pos_eye;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Project;

uniform float time;

void main() {
     vec4 LightPosition = View * vec4(0.0, 0.0, 10000.0, 1.0);
     pos_eye = vec3(View * Model * vPos);
     vec3 normal_eye = vec3(View * Model * vec4(vNormal, 0.0));
     texCoord = vTex;

     fN = normal_eye;
     fE = -pos_eye.xyz;
     fL = LightPosition.xyz;

     if (LightPosition.w != 0.0) {
     	fL = LightPosition.xyz - pos_eye;
     }
     pos = vPos;
     gl_Position = Project * vec4(pos_eye, 1.0);
}
