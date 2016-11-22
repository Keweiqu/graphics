#version 150

in vec4 vPos;
in vec3 vNormal;
in vec2 vTex;

out vec3 N_world; // normal
out vec3 V_world; // to viewer

out vec3 N_eye;
out vec3 V_eye;
out vec3 L_eye;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Project;
uniform vec4 viewPos;

void main() {

     N_world = normalize((Model * vec4(vNormal, 0.0)).xyz);
     V_world = normalize((Model * (viewPos - vPos)).xyz);

     vec4 LightPosition = vec4(0.0, 0.0, 10000.0, 1.0);     
     pos_eye = vec3(View * Model * vPos);
     V_eye = -pos_eye.xyz;	 
     N_eye = vec3(View * Model * vec4(vNormal, 0.0));
     L_eye = (view * LightPosition).xyz;
     if (LightPosition.w != 0.0) {
     	L_eye = (view * (LightPosition - Model * vPos)).xyz;
     }

     gl_Position = Project * View * Model * vPos;
}
