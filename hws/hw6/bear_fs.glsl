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
  vec4 AmbientProduct = vec4(0.65, 0.65, 0.65, 1.0);
  vec4 DiffuseProduct = vec4(0.5, 0.5, 0.5, 1.0);
  vec4 SpecularProduct = vec4(0.5, 0.5, 0.5, 1.0);
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


  fColor = ambient + diffuse + specular;

  vec3 fogc = vec3(1.0, 1.0, 1.0);
  float fogmin = 1000.0, fogmax = 20000.0;
  float dist = length(-pos_eye);
  float fogf = (dist - fogmin) / (fogmax - fogmin);
  fogf = clamp(fogf, 0.0, 1.0);

  //fColor.rgb = mix(fColor.rgb, fogc, fogf);
}
