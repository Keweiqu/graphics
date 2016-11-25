#include "hw6.hpp"

Flock f;
int pause = FALSE, to_left = FALSE, to_right = FALSE;
int up_down = 0, left_right = 0;
GLfloat glTime, glOceanTime;
enum VIEW_TYPE v_mode;
glm::mat4 model_mat, view_mat, project_mat;
GLuint frame_counter = 0, at_night = 0;
glm::vec3 light_position = glm::vec3(-10000, 0.0, 10000.0);
glm::vec3 spotlight_position = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 spotlight_direction = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 cursor_position = glm::vec3(0.0, 0.0, 0.0);
GLfloat spotlight_angle = 30.0;
GLfloat view_angle = DEFAULT_VIEW_ANGLE;
glm::vec3 eye = glm::vec3(1.0);

GLfloat eye_dist = INITIAL_EYE_DIST;
int clicked = FALSE;
meshManager terrain_mesh, terrain_mesh2, ship_mesh, athena_mesh, sphere_mesh, nike_mesh, bear_mesh;

extern GLfloat goal_vertices[24];
extern GLfloat goal_colors[8][4];
extern GLubyte goal_indices[36];
GLfloat boid_vertices[4][3] = {
  {0.0, 0.0, 0.75},
  {0.0, 10, 0.75},
  {-10, -4, 0.75},
  {10, -4, 0.75}
};

GLfloat boid_flap_vertices[4][3] = {
  {0.0, 0.0, 0.75},
  {0.0, 10, 0.75},
  {-8, -3.5, 14.75},
  {8, -3.5, 14.75}
};

GLubyte boid_indices[] = {
  0, 1, 2, // right wing
  3, 1, 0, // left wing
};

GLfloat boid_normals[] = {
  0.0, 0.0, 1.0,
  0.0, 0.0, 1.0,
  0.0, 0.0, 1.0,
  0.0, 0.0, 1.0,
};

GLfloat lighting_conditions[36] = {
  // dawn
  0.4, 0.4, 0.1,
  0.3, 0.3, 0.3,
  0.3, 0.3, 0.3,

  // day
  1.0, 1.0, 1.0,
  1.0, 1.0, 1.0,
  1.0, 1.0, 1.0,

  // dusk
  0.8, 0.1, 0.8,
  0.8, 0.1, 0.8,
  0.8, 0.1, 0.8,

  // night
  0.0, 0.0, 0.4,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0
};

extern GLfloat ocean_vertices[12];
extern GLfloat ocean_tex_coords[8];
extern GLubyte ocean_indices[6];
extern GLfloat ocean_normals[12];

GLuint ocean_shader, boid_shader, terrain_shader, athena_shader, bear_shader;

GLuint boid_vao, boid_vbo1, boid_vbo3, boid_idx, pos, pos1;
GLuint boid_vbo_normal, boid_vbo_tex, boid_normal, boid_texc, feather_tex_sampler, boid_view, boid_project, boid_model;

GLuint terrain_vao, terrain_vbo_pos, terrain_vbo_normal, terrain_vbo_tex, terrain_ebo;

GLuint terrain_vao2, terrain_vbo_pos2, terrain_vbo_normal2, terrain_vbo_tex2, terrain_ebo2;

GLuint terrain_pos, terrain_normal, terrain_tex_coord, terrain_sampler, terrain_sampler1, terrain_view, terrain_project, terrain_model;

GLuint athena_vao, athena_vbo_pos, athena_vbo_normal, athena_ebo, athena_pos, athena_normal, athena_sampler, athena_view_pos, athena_view, athena_project, athena_model;

GLuint nike_vao, nike_vbo_pos, nike_vbo_normal, nike_ebo, nike_pos, nike_normal, nike_sampler, nike_view_pos, nike_view, nike_project, nike_model;

GLuint sphere_vao, sphere_vbo_pos, sphere_vbo_normal, sphere_ebo, sphere_pos, sphere_normal, sphere_sampler, sphere_view_pos, sphere_view, sphere_project, sphere_model;

GLuint bear_vao, bear_vbo_pos, bear_vbo_normal, bear_ebo, bear_pos, bear_normal, bear_view, bear_project, bear_model;

GLuint vao2, ocean_vbo_pos, ocean_vbo_tex, ocean_vbo_index, ocean_vbo_normal;
GLuint ocean_pos, ocean_normal, ocean_texc, ocean_tex_sampler0, ocean_tex_sampler1, model, view, project;

GLuint goal_vao, goal_vbo1, goal_idx;
GLuint spotlight_pos, spotlight_dire;

Image ocean0, ocean1, feather, rock, ice, cube[6];


GLuint t, t2, day_time, light1, light2;
GLuint textures[5], cube_texture;
GLuint light_pos;


static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
  GLuint bufnum;
  glGenBuffers(1, &bufnum);
  glBindBuffer(type, bufnum);
  glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
  return bufnum;
}

void read_images() {
  if (!read_ppm("blue-water3.ppm", &ocean0)) {
    cout << "Fail to read image for ocean0\n" << endl;
    exit(EXIT_FAILURE);
  }

  if (!read_ppm("blue-water4.ppm", &ocean1)) {
    cout << "Fail to read image for ocean1\n" << endl;
    exit(EXIT_FAILURE);
  }

  if (!read_ppm("feather2.ppm", &feather)) {
    cout << "Fail to read image for feather\n" << endl;
    exit(EXIT_FAILURE);
  }

  if(!read_ppm("ice.ppm", &rock)) {
    cout << "Fail to read blue ice image for terrain\n" << endl;
    exit(EXIT_FAILURE);
  }

  if(!read_ppm("snow.ppm", &ice)) {
    cout << "Fail to read white snow image for terrain\n" << endl;
    exit(EXIT_FAILURE);
  }

  //posx, negx, posy, negy, posz, negz
  const char* env_text_names[] = {
    "env/back_cw90.ppm",//back
    "env/front_ccw90.ppm",//front
    "env/right_180.ppm",//right
    "env/left.ppm",//check
    "env/bottom.ppm",//check
    "env/top.ppm"//check
  };
  for(int i = 0; i < 6; i++) {
    if(!read_ppm(env_text_names[i], cube + i)) {
      cout << "Fail to read image " << i << "for cube map" << endl;
      exit(EXIT_FAILURE);
    }
  }
}

void init_ocean() {
  calc_ocean_vertices(WORLD_SIZE * 2);

  glUseProgram(ocean_shader);

  ocean_vbo_pos = make_bo(GL_ARRAY_BUFFER, ocean_vertices, sizeof(ocean_vertices));
  ocean_vbo_tex = make_bo(GL_ARRAY_BUFFER, ocean_tex_coords, sizeof(ocean_tex_coords));
  ocean_vbo_index = make_bo(GL_ELEMENT_ARRAY_BUFFER, ocean_indices, sizeof(ocean_indices));
  ocean_vbo_normal = make_bo(GL_ARRAY_BUFFER, ocean_normals, sizeof(ocean_normals));
  ocean_tex_sampler0 = glGetUniformLocation(ocean_shader, "ocean_tex0");
  ocean_tex_sampler1 = glGetUniformLocation(ocean_shader, "ocean_tex1");
  light1 = glGetUniformLocation(ocean_shader, "light1");
  light2 = glGetUniformLocation(ocean_shader, "light2");
  day_time = glGetUniformLocation(ocean_shader, "day_time");
  light_pos = glGetUniformLocation(ocean_shader, "light_position");
  spotlight_pos = glGetUniformLocation(ocean_shader, "spotlight_position");
  spotlight_dire = glGetUniformLocation(ocean_shader, "spotlight_direction");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3160, 2592, 0, GL_RGB, GL_UNSIGNED_BYTE, ocean0.data);
  glUniform1i(ocean_tex_sampler0, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2804, 2592, 0, GL_RGB, GL_UNSIGNED_BYTE, ocean1.data);
  glUniform1i(ocean_tex_sampler1, 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glGenVertexArrays(1, &vao2);
  glBindVertexArray(vao2);

  glBindBuffer(GL_ARRAY_BUFFER, ocean_vbo_pos);
  ocean_pos = glGetAttribLocation(ocean_shader, "vPos0");
  glEnableVertexAttribArray(ocean_pos);
  glVertexAttribPointer(ocean_pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, ocean_vbo_pos);
  ocean_pos = glGetAttribLocation(ocean_shader, "vPos1");
  glEnableVertexAttribArray(ocean_pos);
  glVertexAttribPointer(ocean_pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ocean_vbo_index);

  glBindBuffer(GL_ARRAY_BUFFER, ocean_vbo_normal);
  ocean_normal = glGetAttribLocation(ocean_shader, "vNormal");
  glEnableVertexAttribArray(ocean_normal);
  glVertexAttribPointer(ocean_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, ocean_vbo_tex);
  ocean_texc = glGetAttribLocation(ocean_shader, "vTex");
  glEnableVertexAttribArray(ocean_texc);
  glVertexAttribPointer(ocean_texc, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  model = glGetUniformLocation(ocean_shader, "Model");
  view = glGetUniformLocation(ocean_shader, "View");
  project = glGetUniformLocation(ocean_shader, "Project");

}

void init_boid() {
  boid_vbo1 = make_bo(GL_ARRAY_BUFFER, boid_vertices, sizeof(boid_vertices));
  boid_vbo3 = make_bo(GL_ARRAY_BUFFER, boid_flap_vertices, sizeof(boid_flap_vertices));
  boid_vbo_normal = make_bo(GL_ARRAY_BUFFER, boid_normals, sizeof(boid_normals));
  boid_vbo_tex = make_bo(GL_ARRAY_BUFFER, ocean_tex_coords, sizeof(ocean_tex_coords));

  boid_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, boid_indices, sizeof(boid_indices));

  glUseProgram(boid_shader);

  //Boid
  feather_tex_sampler = glGetUniformLocation(boid_shader, "feather_tex");
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, feather.data);
  glUniform1i(feather_tex_sampler, 2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glGenVertexArrays(1, &boid_vao);
  glBindVertexArray(boid_vao);
  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo1);
  pos = glGetAttribLocation(boid_shader, "vPos0");
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo3);
  pos1 = glGetAttribLocation(boid_shader, "vPos1");
  glEnableVertexAttribArray(pos1);
  glVertexAttribPointer(pos1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo_normal);
  boid_normal = glGetAttribLocation(boid_shader, "vNormal");
  glEnableVertexAttribArray(boid_normal);
  glVertexAttribPointer(boid_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo_tex);
  boid_texc = glGetAttribLocation(boid_shader, "vTex");
  glEnableVertexAttribArray(boid_texc);
  glVertexAttribPointer(boid_texc, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  boid_view = glGetUniformLocation(boid_shader, "View");
  boid_model = glGetUniformLocation(boid_shader, "Model");
  boid_project = glGetUniformLocation(boid_shader, "Project");
}

void init_goal() {
  glUseProgram(boid_shader);

  goal_vbo1 = make_bo(GL_ARRAY_BUFFER, goal_vertices, sizeof(goal_vertices));
  goal_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, goal_indices, sizeof(goal_indices));

  glGenVertexArrays(1, &goal_vao);
  glBindVertexArray(goal_vao);

  glBindBuffer(GL_ARRAY_BUFFER, goal_vbo1);
  glEnableVertexAttribArray(pos1);
  glVertexAttribPointer(pos1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, goal_vbo1);
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
}

void init_terrain_shader() {
  glUseProgram(terrain_shader);

  terrain_sampler = glGetUniformLocation(terrain_shader, "terrain_tex_blue");
  terrain_sampler1 = glGetUniformLocation(terrain_shader, "terrain_tex_white");

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, textures[3]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rock.sizeX, rock.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, rock.data);
  glUniform1i(terrain_sampler, 3);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, textures[4]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ice.sizeX, ice.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, ice.data);
  glUniform1i(terrain_sampler1, 4);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  terrain_pos = glGetAttribLocation(terrain_shader, "vPos");
  terrain_normal = glGetAttribLocation(terrain_shader, "vNormal");
  terrain_tex_coord = glGetAttribLocation(terrain_shader, "vTex");

  terrain_view = glGetUniformLocation(terrain_shader, "View");
  terrain_model = glGetUniformLocation(terrain_shader, "Model");
  terrain_project = glGetUniformLocation(terrain_shader, "Project");
}

void init_terrain(meshManager &mesh, GLuint* vao, GLuint* vbo_pos, GLuint* vbo_normal, GLuint* vbo_tex, GLuint* ebo) {
   glUseProgram(terrain_shader);

   *vbo_pos = make_bo(GL_ARRAY_BUFFER,
			    &(mesh.vertices->front()),
			    mesh.vertices->size() * sizeof(GLfloat));

   *vbo_normal = make_bo(GL_ARRAY_BUFFER,
			     &(mesh.normals->front()),
			     mesh.normals->size() * sizeof(GLfloat));

   *vbo_tex = make_bo(GL_ARRAY_BUFFER,
			       &(mesh.tex_coords->front()),
			       mesh.tex_coords->size() * sizeof(GLfloat));

   *ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER,
		      &(mesh.indices->front()),
		      mesh.indices->size() * sizeof(GLuint));

  glGenVertexArrays(1, vao);
  glBindVertexArray(*vao);

  glBindBuffer(GL_ARRAY_BUFFER, *vbo_pos);
  glEnableVertexAttribArray(terrain_pos);
  glVertexAttribPointer(terrain_pos, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, *vbo_normal);
  glEnableVertexAttribArray(terrain_normal);
  glVertexAttribPointer(terrain_normal, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, *vbo_tex);
  glEnableVertexAttribArray(terrain_tex_coord);
  glVertexAttribPointer(terrain_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
}

void init_athena() {
  glUseProgram(athena_shader);

  athena_sampler = glGetUniformLocation(athena_shader, "athena_tex");
  athena_view_pos = glGetUniformLocation(athena_shader, "viewPos");
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cube_texture);
  for(int i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
		 0,
		 GL_RGB,
		 cube[i].sizeX,
		 cube[i].sizeY,
		 0,
		 GL_RGB,
		 GL_UNSIGNED_BYTE,
		 cube[i].data);
  }
  glUniform1i(athena_sampler, 5);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  athena_vbo_pos = make_bo(GL_ARRAY_BUFFER, &(athena_mesh.vertices->front()), athena_mesh.vertices->size() * sizeof(GLfloat));
  athena_vbo_normal = make_bo(GL_ARRAY_BUFFER, &(athena_mesh.normals->front()), athena_mesh.normals->size() * sizeof(GLfloat));
  athena_ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER, &(athena_mesh.indices->front()), athena_mesh.indices->size() * sizeof(GLfloat));

  glGenVertexArrays(1, &(athena_vao));
  glBindVertexArray(athena_vao);

  glBindBuffer(GL_ARRAY_BUFFER, athena_vbo_pos);
  athena_pos = glGetAttribLocation(athena_shader, "vPos");
  glEnableVertexAttribArray(athena_pos);
  glVertexAttribPointer(athena_pos, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, athena_vbo_normal);
  athena_normal = glGetAttribLocation(athena_shader, "vNormal");
  glEnableVertexAttribArray(athena_normal);
  glVertexAttribPointer(athena_normal, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, athena_ebo);

  athena_view = glGetUniformLocation(athena_shader, "View");
  athena_model = glGetUniformLocation(athena_shader, "Model");
  athena_project = glGetUniformLocation(athena_shader, "Project");
}

void init_sphere() {
  glUseProgram(athena_shader);

  sphere_sampler = glGetUniformLocation(athena_shader, "athena_tex");
  sphere_view_pos = glGetUniformLocation(athena_shader, "viewPos");
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cube_texture);
  for(int i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
		 0,
		 GL_RGB,
		 cube[i].sizeX,
		 cube[i].sizeY,
		 0,
		 GL_RGB,
		 GL_UNSIGNED_BYTE,
		 cube[i].data);
  }
  glUniform1i(sphere_sampler, 5);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  sphere_vbo_pos = make_bo(GL_ARRAY_BUFFER, &(sphere_mesh.vertices->front()), sphere_mesh.vertices->size() * sizeof(GLfloat));
  sphere_vbo_normal = make_bo(GL_ARRAY_BUFFER, &(sphere_mesh.normals->front()), sphere_mesh.normals->size() * sizeof(GLfloat));
  sphere_ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER, &(sphere_mesh.indices->front()), sphere_mesh.indices->size() * sizeof(GLfloat));

  glGenVertexArrays(1, &(sphere_vao));
  glBindVertexArray(sphere_vao);

  glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo_pos);
  sphere_pos = glGetAttribLocation(athena_shader, "vPos");
  glEnableVertexAttribArray(sphere_pos);
  glVertexAttribPointer(sphere_pos, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo_normal);
  sphere_normal = glGetAttribLocation(athena_shader, "vNormal");
  glEnableVertexAttribArray(sphere_normal);
  glVertexAttribPointer(sphere_normal, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);

  sphere_view = glGetUniformLocation(athena_shader, "View");
  sphere_model = glGetUniformLocation(athena_shader, "Model");
  sphere_project = glGetUniformLocation(athena_shader, "Project");
}

void init_nike() {
  glUseProgram(bear_shader);

  nike_sampler = glGetUniformLocation(athena_shader, "athena_tex");
  nike_view_pos = glGetUniformLocation(athena_shader, "viewPos");
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cube_texture);
  for(int i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
		 0,
		 GL_RGB,
		 cube[i].sizeX,
		 cube[i].sizeY,
		 0,
		 GL_RGB,
		 GL_UNSIGNED_BYTE,
		 cube[i].data);
  }
  glUniform1i(nike_sampler, 5);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  nike_vbo_pos = make_bo(GL_ARRAY_BUFFER, &(nike_mesh.vertices->front()), nike_mesh.vertices->size() * sizeof(GLfloat));
  nike_vbo_normal = make_bo(GL_ARRAY_BUFFER, &(nike_mesh.normals->front()), nike_mesh.normals->size() * sizeof(GLfloat));
  nike_ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER, &(nike_mesh.indices->front()), nike_mesh.indices->size() * sizeof(GLfloat));

  glGenVertexArrays(1, &(nike_vao));
  glBindVertexArray(nike_vao);

  glBindBuffer(GL_ARRAY_BUFFER, nike_vbo_pos);
  nike_pos = glGetAttribLocation(athena_shader, "vPos");
  glEnableVertexAttribArray(nike_pos);
  glVertexAttribPointer(nike_pos, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, nike_vbo_normal);
  nike_normal = glGetAttribLocation(athena_shader, "vNormal");
  glEnableVertexAttribArray(nike_normal);
  glVertexAttribPointer(nike_normal, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nike_ebo);

  nike_view = glGetUniformLocation(athena_shader, "View");
  nike_model = glGetUniformLocation(athena_shader, "Model");
  nike_project = glGetUniformLocation(athena_shader, "Project");
}

void init_bear() {
  glUseProgram(bear_shader);

  bear_vbo_pos = make_bo(GL_ARRAY_BUFFER, &(bear_mesh.vertices->front()), bear_mesh.vertices->size() * sizeof(GLfloat));
  bear_vbo_normal = make_bo(GL_ARRAY_BUFFER, &(bear_mesh.normals->front()), bear_mesh.normals->size() * sizeof(GLfloat));
  bear_ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER, &(bear_mesh.indices->front()), bear_mesh.indices->size() * sizeof(GLfloat));

  glGenVertexArrays(1, &(bear_vao));
  glBindVertexArray(bear_vao);

  glBindBuffer(GL_ARRAY_BUFFER, bear_vbo_pos);
  bear_pos = glGetAttribLocation(athena_shader, "vPos");
  glEnableVertexAttribArray(bear_pos);
  glVertexAttribPointer(bear_pos, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, bear_vbo_normal);
  bear_normal = glGetAttribLocation(athena_shader, "vNormal");
  glEnableVertexAttribArray(bear_normal);
  glVertexAttribPointer(bear_normal, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bear_ebo);

  bear_view = glGetUniformLocation(athena_shader, "View");
  bear_model = glGetUniformLocation(athena_shader, "Model");
  bear_project = glGetUniformLocation(athena_shader, "Project");
}

void init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  read_images();

  glGenTextures(4, textures);
  ocean_shader = initshader("ocean_vs.glsl", "ocean_fs.glsl");
  boid_shader = initshader("boid_vs.glsl", "boid_fs.glsl");
  terrain_shader = initshader("terrain_vs.glsl", "terrain_fs.glsl");
  athena_shader = initshader("athena_vs.glsl", "athena_fs.glsl");
  bear_shader = initshader("bear_vs.glsl", "bear_fs.glsl");

  init_boid();
  init_goal();
  init_ocean();
  init_terrain_shader();
  init_terrain(terrain_mesh, &terrain_vao, &terrain_vbo_pos, &terrain_vbo_normal, &terrain_vbo_tex, &terrain_ebo);
  init_terrain(terrain_mesh2, &terrain_vao2, &terrain_vbo_pos2, &terrain_vbo_normal2, &terrain_vbo_tex2, &terrain_ebo2);
  init_athena();
  init_sphere();
  init_nike();
  init_bear();
  init_time();

  glClearColor(0.0, 0.0, 0.0, 1.0);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_EQUAL:
        f.add_boid();
        break;
      case GLFW_KEY_BACKSPACE:
        f.remove_boid();
        break;
    case 'P':
    case 'p':
      pause = !pause;
      print_step_msg(&f);
      break;
    case 'd':
    case 'D':
      print_step_msg(&f);
      if(!pause) {
	pause = TRUE;
      } else {
	f.update_centers();
	f.update_ave_v();
	f.update_goal();
	f.update_boids();
      }
      break;

    case GLFW_KEY_LEFT:
      to_left = TRUE;
      to_right = FALSE;
      break;
    case GLFW_KEY_RIGHT:
      to_right = TRUE;
      to_left= FALSE;
      break;
      case GLFW_KEY_Q:
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(w, true);
        break;
      case GLFW_KEY_C:
        v_mode = CENTER;
        break;
      case GLFW_KEY_T:
        v_mode = TRAILING;
        break;
      case GLFW_KEY_S:
        v_mode = SIDE;
        break;
      case GLFW_KEY_F:
        v_mode = FIRST_PERSON;
        break;
    case GLFW_KEY_COMMA:
      if(f.speed > 5) {
	f.speed -= 1.0;
      }
      break;
    case GLFW_KEY_PERIOD:
      if(f.speed < 20) {
	f.speed += 1.0;
      }
      break;
    case GLFW_KEY_N:
      view_angle = DEFAULT_VIEW_ANGLE;
      project_mat = glm::perspective(view_angle * DEGREE_TO_RADIAN, 1.0, 1.0, 100000.0);
      break;
    }
  }

  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch (key) {
      case GLFW_KEY_I:
        zoom_in();
        break;
      case GLFW_KEY_O:
        zoom_out();
        break;
    }
  }
  if(action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch(key) {
    case GLFW_KEY_UP:
      if(up_down < 50) {
	up_down += 1;
      }
      break;
    case GLFW_KEY_DOWN:
    if(up_down > -50) {
      up_down -= 1;
    }
    default:
      break;
    }
  }

}

/*
glm::vec3 MouseToWorld(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    return glm::vec3(posX * WORLD_SIZE, posY * WORLD_SIZE, 0.0);
}

void cursor(GLFWwindow* window, double xpos, double ypos) {
  cursor_position = MouseToWorld((int)xpos, (int)ypos);
}
*/
void reshape(GLFWwindow *w, int width, int height) {
  glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
}

void framebuffer_resize(GLFWwindow *w, int width, int height) {
  glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
  v_mode = SIDE;
  eye = glm::vec3(0.0, -50000.0, 10.0);
  glm::vec3 center = glm::vec3(5000.0, 0.0, 2000.0);
  glm::vec3 up = glm::vec3(0, 0, 1);
  view_mat = glm::lookAt(eye, center, up);
  project_mat = glm::perspective(view_angle * DEGREE_TO_RADIAN, 1.0, 1.0, 100000.0);

  if(!glfwInit()) {
    cerr << "Error: cannot start GLFW3" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(1000, 1000, "Triangle", NULL, NULL);
  if(!window) {
    cerr << "Error: Cannot open window with GLFW3" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  glewInit();

  glfwSetKeyCallback(window, keyboard);
  //glfwSetCursorPosCallback(window, cursor);
  glfwSetWindowSizeCallback(window, reshape);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);

  terrain_mesh.readFile("terrain.off");
  terrain_mesh.scale = 2.0;
  terrain_mesh.trans_vec = glm::vec3(-8500.0, 0.0, -15000.0);

  terrain_mesh2.readFile("terrain2.off");
  terrain_mesh2.scale = 2.0;
  terrain_mesh2.trans_vec = glm::vec3(7500, 0.0, -5000.0);

  ship_mesh.readFile("ship.off");
  ship_mesh.scale = 1.0;
  ship_mesh.trans_vec = glm::vec3(0.0, 0.0, -8000.0);

  athena_mesh.readFile("meshes/athena.off");
  athena_mesh.scale = 2.0; //0.6
  athena_mesh.trans_vec = glm::vec3(-1000.0, 6500.0, 4500.0);
  athena_mesh.rotate_angles = glm::vec3(90 * DEGREE_TO_RADIAN, 0.0, 0.0);

  sphere_mesh.readFile("meshes/sphere2.off");
  sphere_mesh.scale = 100.0;
  sphere_mesh.trans_vec = glm::vec3(-1000.0, 8500.0, 4500.0);
  sphere_mesh.rotate_angles = glm::vec3(90 * DEGREE_TO_RADIAN, 0.0, 0.0);

  nike_mesh.readFile("meshes/nike.off");
  nike_mesh.scale = 100.0;
  nike_mesh.trans_vec = glm::vec3(12000.0, 5500.0, 3500.0);
  nike_mesh.rotate_angles = glm::vec3(90 * DEGREE_TO_RADIAN, 0.0, 0.0);

  bear_mesh.readFile("meshes/bear.off");
  bear_mesh.scale = 100.0;
  bear_mesh.trans_vec = glm::vec3(15500.0, 3500.0, 6500.0);
  bear_mesh.rotate_angles = glm::vec3(0.0, 0.0, 90 * DEGREE_TO_RADIAN);

  init();

  GLfloat angle = 0.0;
  GLfloat radius = 15000.0;
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    update_frame_counter();
    update_light_position();
    update_view(view_mat, f);
    eye = glm::vec3(sin(angle) * radius, cos(angle) * radius, 10000.0);
    view_mat = glm::lookAt(eye, center, up);
    angle += 0.005;
    draw_terrain(terrain_mesh, terrain_vao, terrain_ebo);
    draw_terrain(terrain_mesh2, terrain_vao2, terrain_ebo2);
    // draw_ship();
    draw_athena();
    draw_sphere();
    draw_nike();
    draw_bear();
    draw_ocean(vao2);
    draw_flock(&f, model, boid_vao, boid_idx);
    draw_goal(&f, model, goal_vao, goal_idx);
    if(!pause) {
      f.update_centers();
      f.update_ave_v();
      f.update_goal();
      f.update_boids();
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}
