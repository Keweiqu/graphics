#version 150

in vec4 vPos;
in vec3 vNormal;
out vec4 color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Project;
void main() {
     vec4 LightPosition = View * vec4(50.0, 100, 0.0, 1.0);
     vec4 AmbientProduct = vec4(0.0215, 0.1745, 0.0215, 1.0);
     vec4 DiffuseProduct = vec4(0.07568, 0.61424, 0.0756, 1.0);
     vec4 SpecularProduct = vec4(0.633, 0.727811, 0.633, 1.0);
     float Shininess = 0.6 * 128;

     vec3 pos = (View * Model * vPos).xyz;
     vec3 L = normalize(LightPosition.xyz - pos);
     vec3 E = normalize(-pos);
     vec3 H = normalize(L + E);
     vec3 N = normalize(View * Model * vec4(vNormal, 0.0)).xyz;

     vec4 ambient = AmbientProduct;
     float Kd = max(dot(L, N), 0.0);
     vec4 diffuse = Kd * DiffuseProduct;
     float Ks = pow(max(dot(N, H), 0.0), Shininess);
     vec4 specular = Ks * SpecularProduct;
     if(dot(L, N) < 0.0)
     	       specular = vec4(0.0, 0.0, 0.0, 0.0);
     gl_Position = Project * View * Model * vPos;
     color = ambient + diffuse + specular;
     color.a = 1.0;
}
