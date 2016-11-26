#version 150

in vec4 vPos;
in vec3 vNormal;
in vec2 vTex;

out vec4 P;
out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec3 pos_eye;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Project;

uniform float time;
uniform vec3 light_position;
uniform vec3 spotlight_position;
uniform int atNight;

void main() {
     P = vPos;
     vec4 LightPosition = View * vec4(light_position.x, light_position.y, light_position.z, 1.0);
     if (atNight == 1) {
      LightPosition = View * vec4(spotlight_position.x, spotlight_position.y, spotlight_position.z, 1.0);
     }
     pos_eye = vec3(View * Model * vPos);
     vec3 normal_eye = vec3(View * Model * vec4(vNormal, 0.0));

     fN = normal_eye;
     fE = -pos_eye.xyz;
     fL = LightPosition.xyz;

     if (LightPosition.w != 0.0) {
     	fL = LightPosition.xyz - pos_eye;
     }

     gl_Position = Project * vec4(pos_eye, 1.0);
}
