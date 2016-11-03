#version 150

out vec4 fColor;
in vec3 fN;
in vec3 fL;
in vec3 fE;

uniform mat4 Model;
uniform mat4 View;

void main() {
     vec4 AmbientProduct = vec4(0.0215, 0.1745, 0.0215, 1.0);
     vec4 DiffuseProduct = vec4(0.07568, 0.61424, 0.0756, 1.0);
     vec4 SpecularProduct = vec4(0.633, 0.727811, 0.633, 1.0);
     float Shininess = 0.6 * 128;

     vec3 N = normalize(fN);
     vec3 E = normalize(fE);
     vec3 L = normalize(fL);
     vec3 H = normalize(L + E);

     vec4 ambient = AmbientProduct;

     float Kd = max(dot(L, N), 0.0);
     vec4 diffuse = Kd * DiffuseProduct;

     float Ks = pow(max(dot(N, H), 0.0), Shininess);
     vec4 specular = Ks * SpecularProduct;
     if(dot(L, N) < 0.0) {
     	specular = vec4(0.0, 0.0, 0.0, 1.0);
     }

     fColor = ambient + diffuse + specular;
     fColor.a = 1.0;
}
