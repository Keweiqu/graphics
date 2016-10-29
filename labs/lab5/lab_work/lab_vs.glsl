#version 150

in vec4 vPos;
in vec3 vNormal;

out vec3 fN;
out vec3 fE;
out vec3 fL;

uniform mat4 ModelView;
uniform mat4 Project;

void main() {
     vec4 LightPosition = ModelView * vec4(0.0, 5.0, 0.0, 1.0);
     vec4 pos = ModelView * vPos;

     fN = (ModelView * vec4(vNormal, 0.0)).xyz;
     fE = pos.xyz;
     fL = LightPosition.xyz;

     if( LightPosition.w != 0.0) {
     	 fL = LightPosition.xyz - pos.xyz;
     }
     gl_Position = Project * ModelView * vPos;
}
