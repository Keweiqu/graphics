#version 150

in vec4 vPos;
in vec3 vNormal;

out vec3 N_world; // normal
out vec3 V_world; // to viewer

out vec3 N_eye;
out vec3 V_eye;
out vec3 L_eye;
out vec4 P;

uniform int atNight;
uniform vec3 light_position;
uniform vec3 spotlight_position;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Project;
uniform vec4 viewPos;


void main() {
     P = vPos;
     N_world = normalize((Model * vec4(vNormal, 0.0)).xyz);
     V_world = normalize((viewPos - Model * vPos).xyz);

     vec4 LightPosition = vec4(light_position.x, light_position.y, light_position.z, 1.0);
     if (atNight == 1) {
      LightPosition = vec4(spotlight_position.x, spotlight_position.y, spotlight_position.z, 1.0);
     }

     vec3 pos_eye = (View * Model * vPos).xyz;
     V_eye = -pos_eye.xyz;
     N_eye = vec3(View * Model * vec4(vNormal, 0.0));
     L_eye = (View * LightPosition).xyz;
     if (LightPosition.w != 0.0) {
     	L_eye = (View * (LightPosition - Model * vPos)).xyz;
     }

     gl_Position = Project * View * Model * vPos;
}
