#version 150

out vec4 fColor;
in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec3 pos_eye, normal_eye;
in vec2 texCoord;
in vec4 P;
uniform sampler2D ocean_tex0;
uniform sampler2D ocean_tex1;
uniform float ocean_time;
uniform float day_time;
uniform mat3 light1;
uniform mat3 light2;

uniform vec3 spotlight_position;
uniform vec3 spotlight_direction;
//uniform vec3 cursor_position;
//uniform float spotlight_angle;
uniform int atNight;

uniform mat4 Model;
//uniform mat4 spotlightModel;

void main() {
  vec4 AmbientProduct = vec4(light1[0].x * (1 - day_time) + light2[0].x * day_time,
                             light1[0].y * (1 - day_time) + light2[0].y * day_time,
                             light1[0].z * (1 - day_time) + light2[0].z * day_time,
                             1.0);
  vec4 DiffuseProduct = vec4(light1[1].x * (1 - day_time) + light2[1].x * day_time,
                             light1[1].y * (1 - day_time) + light2[1].y * day_time,
                             light1[1].z * (1 - day_time) + light2[1].z * day_time,
                             1.0);
  vec4 SpecularProduct = vec4(light1[2].x * (1 - day_time) + light2[2].x * day_time,
                              light1[2].y * (1 - day_time) + light2[2].y * day_time,
                              light1[2].z * (1 - day_time) + light2[2].z * day_time,
                              1.0);
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

  float attenuation = 0.0;
  vec3 cone_direction = normalize(spotlight_direction);
  //vec3 cone_direction = normalize((spotlightModel * vec4(cursor_position, 1.0)).xyz - spotlight_position);
  vec3 ray_direction = normalize((Model * P).xyz - spotlight_position);
  float ray_angle = degrees(acos(dot(ray_direction, cone_direction)));
  if (ray_angle < 10) {
    attenuation = 1 - ray_angle / 10;
  }


  vec4 shadeLight = ambient + diffuse + specular;
  if (atNight == 1) {
    shadeLight += attenuation * vec4(1.0, 1.0, 1.0, 1.0);
  }
  shadeLight.a = 1.0;
  vec4 shadeTex = vec4(mix(texture(ocean_tex0, texCoord).rgb, texture(ocean_tex1, texCoord).rgb, ocean_time), 1.0);
  fColor = shadeLight * shadeTex;
}
