#version 150

//out vec4 fColor;

//in vec3 N_world;
//in vec3 V_world;

//in vec3 N_eye;
//in vec3 V_eye;
//in vec3 L_eye;

//uniform samplerCube athena_tex;

//void main() {
//  vec4 AmbientProduct = vec4(1.0, 1.0, 1.0, 1.0);
//  vec4 DiffuseProduct = vec4(1.0, 1.0, 1.0, 1.0);
//  vec4 SpecularProduct = vec4(1.0, 1.0, 1.0, 1.0);
//  float Shininess = 0.9 * 128;

//  vec3 N = normalize(N_eye);
//  vec3 V = normalize(V_eye);
//  vec3 L = normalize(L_eye);
//  vec3 H = normalize(L + V);

//  vec3 R = reflect(V_world, N_world);
//  vec4 texColor = reflect(athena_tex, R);


//  vec4 ambient = AmbientProduct;

//  float Kd = max(dot(L, N), 0.0);
//  vec4 diffuse = Kd * DiffuseProduct;

//  float Ks = pow(max(dot(N, H), 0.0), Shininess);
//  vec4 specular = Ks * SpecularProduct;
//  if (dot(L, N) < 0.0) {
//    specular = vec4(0.0, 0.0, 0.0, 1.0);
//  }

//  vec4 shadeLight = ambient + diffuse + specular;
//  shadeLight.a = 1.0;

//  fColor = shadeLight * texColor;
//}

out vec4 fColor;
in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec2 texCoord;
uniform sampler2D feather_tex;

void main() {
  vec4 AmbientProduct = vec4(0.2125, 0.1275, 0.054, 1.0);
  vec4 DiffuseProduct = vec4(0.714, 0.4284, 0.18144, 1.0);
  vec4 SpecularProduct = vec4(0.393548, 0.271906, 0.166721, 1.0);
  float Shininess = 0.2 * 128;

  vec3 N = normalize(fN);
  vec3 E = normalize(fE);
  vec3 L = normalize(fL);
  vec3 H = normalize(L + E);

  vec4 ambient = AmbientProduct;

  float Kd = max(dot(L, N), 0.0);
  vec4 diffuse = Kd * DiffuseProduct;

  float Ks = pow(max(dot(N, H), 0.0), Shininess);
  vec4 specular = Ks * SpecularProduct;
  if (dot(L, N) < 0.0) {
    specular = vec4(0.0, 0.0, 0.0, 1.0);
  }

  //vec4 shadeLight = ambient + diffuse + specular;
  //shadeLight.a = 1.0;
  //vec4 shadeTex = vec4(texture(feather_tex, texCoord).rgb, 1.0);
  //fColor = shadeLight * shadeTex;
  fColor = ambient + diffuse + specular;
  fColor.a = 1.0;
}
