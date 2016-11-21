#version 150

out vec4 fColor;
in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec3 pos_eye, normal_eye;
in vec2 texCoord;
uniform sampler2D ocean_tex0;
uniform sampler2D ocean_tex1;
uniform float ocean_time;
uniform mat3 light1;
uniform mat3 light2;

void main() {
  vec4 AmbientProduct = vec4(light1[0].x, light1[0].y, light1[0].z, 1.0);
  vec4 DiffuseProduct = vec4(light1[1].x, light1[1].y, light1[1].z, 1.0);
  vec4 SpecularProduct = vec4(light1[2].x, light1[2].y, light1[2].z, 1.0);
  float Shininess = 0.9 * 128;

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

  vec4 shadeLight = ambient + diffuse + specular;
  shadeLight.a = 1.0;
  vec4 shadeTex = vec4(mix(texture(ocean_tex0, texCoord).rgb, texture(ocean_tex1, texCoord).rgb, ocean_time), 1.0);
  fColor = shadeLight * shadeTex;
}
