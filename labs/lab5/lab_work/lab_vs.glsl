#version 150

in vec4 vPos;
in vec3 vNormal;

out vec3 fN;
out vec3 fE;
out vec3 fL;

uniform mat4 ModelView;
uniform mat4 Project;

void main() {
     vec4 LightPosition = vec4(0.0, 5.0, 0.0, 1.0);
   

     fN = vNormal;
     fE = vPos.xyz;
     fL = LightPosition.xyz;

     if( LightPosition.w != 0.0) {
     	 fL = LightPosition.xyz - vPos.xyz;
     }
     gl_Position = Project * ModelView * vPos;
}
