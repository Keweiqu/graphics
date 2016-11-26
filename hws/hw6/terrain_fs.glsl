#version 150

out vec4 fColor;
in vec4 pos_model;
in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec3 pos_eye;
in vec2 texCoord;
uniform sampler2D terrain_tex_blue;
uniform sampler2D terrain_tex_white;

void main() {
  vec4 AmbientProduct = vec4(1.0, 1.0, 1.0, 1.0);
  vec4 DiffuseProduct = vec4(1.0, 1.0, 1.0, 1.0);
  vec4 SpecularProduct = vec4(1.0, 1.0, 1.0, 1.0);
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
  vec4 shadeTex = vec4(mix(texture(terrain_tex_blue, texCoord).rgb, texture(terrain_tex_white, texCoord).rgb, pos_model.z / 17000), 1.0);
  fColor = shadeLight * shadeTex;

  vec3 fogc = vec3(0.3, 0.3, 0.3);
  float fogmin = 1000.0, fogmax = 20000.0;
  float dist = length(-pos_eye);
  float fogf = (dist - fogmin) / (fogmax - fogmin);
  fogf = clamp(fogf, 0.0, 1.0);

  //fColor.rgb = mix(fColor.rgb, fogc, fogf);
}
