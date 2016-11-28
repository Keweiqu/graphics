#include "hw6.hpp"

Flock f;
int pause = FALSE, to_left = FALSE, to_right = FALSE;
int up_down = 0, left_right = 0;
GLfloat glTime, glOceanTime;
enum VIEW_TYPE v_mode;
GLfloat st_zoom_factor, fp_zoom_factor;
glm::mat4 model_mat, view_mat, project_mat;
GLuint frame_counter = 0, at_night = 0;
glm::vec3 light_position = glm::vec3(-10000, 0.0, 10000.0);
glm::vec3 spotlight_position = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 spotlight_direction = glm::vec3(1.0, 1.0, 1.0);
GLfloat spotlight_angle = 30.0;
GLfloat view_angle = DEFAULT_VIEW_ANGLE;
glm::vec3 eye_pos = glm::vec3(1.0);
glm::vec3 look_pos = glm::vec3(0.0, 0.0, 2000.0);
glm::vec3 eye_trans = glm::vec3(0.0, 0.0, 0.0);

meshManager terrain_mesh, terrain_mesh_s, terrain_mesh2, terrain_mesh2_s, athena_mesh, sphere_mesh, nike_mesh, bear_mesh;

extern GLfloat goal_vertices[24];
extern GLfloat goal_colors[8][4];
extern GLubyte goal_indices[36];
extern GLfloat flight_centers[6];
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
extern GLfloat box_vertices[24];

GLuint ocean_shader, boid_shader, terrain_shader, athena_shader, bear_shader;

GLuint boid_vao, boid_vbo1, boid_vbo3, boid_idx, pos, pos1;
GLuint boid_vbo_normal, boid_vbo_tex, boid_normal, boid_texc, feather_tex_sampler, boid_view, boid_project, boid_model;

GLuint terrain_vao, terrain_vbo_pos, terrain_vbo_normal, terrain_vbo_tex, terrain_ebo;

GLuint terrain_s_vao, terrain_s_vbo_pos, terrain_s_vbo_normal, terrain_s_vbo_tex, terrain_s_ebo;

GLuint terrain_vao2, terrain_vbo_pos2, terrain_vbo_normal2, terrain_vbo_tex2, terrain_ebo2;

GLuint terrain_s_vao2, terrain_s_vbo_pos2, terrain_s_vbo_normal2, terrain_s_vbo_tex2, terrain_s_ebo2;

GLuint terrain_pos, terrain_normal, terrain_tex_coord, terrain_sampler, terrain_sampler1, terrain_view, terrain_project, terrain_model;

GLuint athena_vao, athena_vbo_pos, athena_vbo_normal, athena_ebo, athena_pos, athena_normal, athena_sampler, athena_view_pos, athena_view, athena_project, athena_model;

GLuint nike_vao, nike_vbo_pos, nike_vbo_normal, nike_ebo;

GLuint sphere_vao, sphere_vbo_pos, sphere_vbo_normal, sphere_ebo;

GLuint bear_vao, bear_vbo_pos, bear_vbo_normal, bear_ebo, bear_pos, bear_normal, bear_view, bear_project, bear_model;

GLuint vao2, ocean_vbo_pos, ocean_vbo_tex, ocean_vbo_index, ocean_vbo_normal;
GLuint ocean_pos, ocean_normal, ocean_texc, ocean_tex_sampler0, ocean_tex_sampler1, model, view, project;

GLuint goal_vao, goal_vbo1, goal_idx;
GLuint spotlight_pos, spotlight_dire;

Image ocean0, ocean1, feather, rock, ice, cube[6], box[5];

GLuint t, t2, light1, light2;
GLuint textures[5], cube_texture, box_texture[5];
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
  const char* box_text_names[] = {
    "env/front.ppm",
    "env/back_180.ppm",
    "env/left_cw90.ppm",
    "env/right_ccw90.ppm",
    "env/top_cw90.ppm"
  };
  for(int i = 0; i < 6; i++) {
    if(!read_ppm(env_text_names[i], cube + i)) {
      cout << "Fail to read image " << i << "for cube map" << endl;
      exit(EXIT_FAILURE);
    }
  }
  for (int i = 0; i < 5; i++) {
    if (!read_ppm(box_text_names[i], box + i)) {
      cout << "Fail to read image " << i << "for box" << endl;
      exit(EXIT_FAILURE);
    }
  }
}

void init_box_textures() {
  for (int i = 0; i < 5; i++) {
    glActiveTexture(GL_TEXTURE6 + i);
    glBindTexture(GL_TEXTURE_2D, box_texture[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, box[i].data);
    glUniform1i(ocean_tex_sampler0, 6 + i);
    glUniform1i(ocean_tex_sampler1, 6 + i);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
  light_pos = glGetUniformLocation(ocean_shader, "light_position");
  spotlight_pos = glGetUniformLocation(ocean_shader, "spotlight_position");
  spotlight_dire = glGetUniformLocation(ocean_shader, "spotlight_direction");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ocean0.sizeX, ocean0.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, ocean0.data);
  glUniform1i(ocean_tex_sampler0, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ocean1.sizeX, ocean1.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, ocean1.data);
  glUniform1i(ocean_tex_sampler1, 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  init_box_textures();

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

void init_athena_shader() {
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

  athena_view = glGetUniformLocation(athena_shader, "View");
  athena_model = glGetUniformLocation(athena_shader, "Model");
  athena_project = glGetUniformLocation(athena_shader, "Project");

}

void init_athena() {
  glUseProgram(athena_shader);

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
}

void init_sphere() {
  glUseProgram(athena_shader);

  sphere_vbo_pos = make_bo(GL_ARRAY_BUFFER, &(sphere_mesh.vertices->front()), sphere_mesh.vertices->size() * sizeof(GLfloat));
  sphere_vbo_normal = make_bo(GL_ARRAY_BUFFER, &(sphere_mesh.normals->front()), sphere_mesh.normals->size() * sizeof(GLfloat));
  sphere_ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER, &(sphere_mesh.indices->front()), sphere_mesh.indices->size() * sizeof(GLfloat));

  glGenVertexArrays(1, &(sphere_vao));
  glBindVertexArray(sphere_vao);

  glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo_pos);
  glEnableVertexAttribArray(athena_pos);
  glVertexAttribPointer(athena_pos, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo_normal);
  glEnableVertexAttribArray(athena_normal);
  glVertexAttribPointer(athena_normal, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);
 }

void init_nike() {
  glUseProgram(athena_shader);

  nike_vbo_pos = make_bo(GL_ARRAY_BUFFER, &(nike_mesh.vertices->front()), nike_mesh.vertices->size() * sizeof(GLfloat));
  nike_vbo_normal = make_bo(GL_ARRAY_BUFFER, &(nike_mesh.normals->front()), nike_mesh.normals->size() * sizeof(GLfloat));
  nike_ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER, &(nike_mesh.indices->front()), nike_mesh.indices->size() * sizeof(GLfloat));

  glGenVertexArrays(1, &(nike_vao));
  glBindVertexArray(nike_vao);

  glBindBuffer(GL_ARRAY_BUFFER, nike_vbo_pos);
  glEnableVertexAttribArray(athena_pos);
  glVertexAttribPointer(athena_pos, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, nike_vbo_normal);
  glEnableVertexAttribArray(athena_normal);
  glVertexAttribPointer(athena_normal, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nike_ebo);
}

void init_bear() {
  glUseProgram(bear_shader);

  bear_vbo_pos = make_bo(GL_ARRAY_BUFFER, &(bear_mesh.vertices->front()), bear_mesh.vertices->size() * sizeof(GLfloat));
  bear_vbo_normal = make_bo(GL_ARRAY_BUFFER, &(bear_mesh.normals->front()), bear_mesh.normals->size() * sizeof(GLfloat));
  bear_ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER, &(bear_mesh.indices->front()), bear_mesh.indices->size() * sizeof(GLfloat));

  glGenVertexArrays(1, &(bear_vao));
  glBindVertexArray(bear_vao);

  glBindBuffer(GL_ARRAY_BUFFER, bear_vbo_pos);
  bear_pos = glGetAttribLocation(bear_shader, "vPos");
  glEnableVertexAttribArray(bear_pos);
  glVertexAttribPointer(bear_pos, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, bear_vbo_normal);
  bear_normal = glGetAttribLocation(bear_shader, "vNormal");
  glEnableVertexAttribArray(bear_normal);
  glVertexAttribPointer(bear_normal, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bear_ebo);

  bear_view = glGetUniformLocation(bear_shader, "View");
  bear_model = glGetUniformLocation(bear_shader, "Model");
  bear_project = glGetUniformLocation(bear_shader, "Project");
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
  init_terrain(terrain_mesh,
	       &terrain_vao,
	       &terrain_vbo_pos,
	       &terrain_vbo_normal,
	       &terrain_vbo_tex,
	       &terrain_ebo);
  init_terrain(terrain_mesh_s,
	       &terrain_s_vao,
	       &terrain_s_vbo_pos,
	       &terrain_s_vbo_normal,
	       &terrain_s_vbo_tex,
	       &terrain_s_ebo);
  init_terrain(terrain_mesh2,
	       &terrain_vao2,
	       &terrain_vbo_pos2,
	       &terrain_vbo_normal2,
	       &terrain_vbo_tex2,
	       &terrain_ebo2);
  init_terrain(terrain_mesh2_s,
	       &terrain_s_vao2,
	       &terrain_s_vbo_pos2,
	       &terrain_s_vbo_normal2,
	       &terrain_s_vbo_tex2,
	       &terrain_s_ebo2);
  init_athena_shader();
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
    case 'P':
    case 'p':
      pause = !pause;
      print_step_msg(&f);
      break;
    case 'w':
    case 'W':
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

    case GLFW_KEY_A:
      to_left = TRUE;
      to_right = FALSE;
      break;
    case GLFW_KEY_D:
      to_right = TRUE;
      to_left= FALSE;
      break;
    case GLFW_KEY_Q:
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(w, true);
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
    case GLFW_KEY_E:
      v_mode = FREE;
      break;
    case GLFW_KEY_N:
      st_zoom_factor = 0;
      fp_zoom_factor = 0;
      break;
    case GLFW_KEY_0:
      f.sequence = DEFAULT;
      break;
    case GLFW_KEY_1:
      if(f.sequence != ATHENA) {
	f.in_range = FALSE;
      }
      f.sequence = ATHENA;
      break;
    case GLFW_KEY_2:
      if(f.sequence != NIKE) {
	f.in_range = FALSE;
      }
      f.sequence = NIKE;
      break;
    case GLFW_KEY_3:
      if(f.sequence != BEAR) {
	f.in_range = FALSE;
      }
      f.sequence = BEAR;
      break;
    }
  }

  if(action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch(key) {
    case GLFW_KEY_UP:
      if(up_down < 40) {
	up_down += 1;
      }
      break;
    case GLFW_KEY_DOWN:
      if(up_down > 0) {
	up_down -= 1;
      }
      break;
    case GLFW_KEY_RIGHT:
      if(left_right < 15) {
	left_right += 1;
      }
      break;
    case GLFW_KEY_LEFT:
      if(left_right > -15) {
	left_right -= 1;
      }
      break;
    case GLFW_KEY_I:
      zoom_in();
      break;
    case GLFW_KEY_O:
      zoom_out();
      break;
    default:
      break;
    }
  }
}

void reshape(GLFWwindow *w, int width, int height) {
  glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
}

void framebuffer_resize(GLFWwindow *w, int width, int height) {
  glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
  v_mode = SIDE;
  st_zoom_factor = 0.0;
  fp_zoom_factor = 0.0;
  project_mat = glm::perspective(DEFAULT_VIEW_ANGLE * DEGREE_TO_RADIAN, 1.0, 10.0, 100000.0);

  if(!glfwInit()) {
    cerr << "Error: cannot start GLFW3" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(1000, 1000, "Island World", NULL, NULL);
  if(!window) {
    cerr << "Error: Cannot open window with GLFW3" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  glewInit();

  glfwSetKeyCallback(window, keyboard);
  glfwSetWindowSizeCallback(window, reshape);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);

  terrain_mesh.readFile("terrain.off");
  terrain_mesh.scale = 2.0;
  terrain_mesh.trans_vec = glm::vec3(-20000.0, 0.0, -15000.0);

  terrain_mesh_s.readFile("s_terrain.off");
  terrain_mesh_s.scale = 2.0;
  terrain_mesh_s.trans_vec = glm::vec3(-20000.0, 0.0, -15000.0);

  terrain_mesh2.readFile("terrain3.off");
  terrain_mesh2.scale = 2.0;
  terrain_mesh2.trans_vec = glm::vec3(6500, 0.0, -6000.0);

  terrain_mesh2_s.readFile("s_terrain3.off");
  terrain_mesh2_s.scale = 2.0;
  terrain_mesh2_s.trans_vec = glm::vec3(6500, 0.0, -6000.0);

  athena_mesh.readFile("meshes/athena.off");
  athena_mesh.scale = 2.0; //0.6
  athena_mesh.trans_vec = glm::vec3(-12500.0, 6500.0, 5500.0);
  athena_mesh.rotate_angles = glm::vec3(90 * DEGREE_TO_RADIAN, 0.0, 0.0);

  sphere_mesh.readFile("meshes/sphere2.off");
  sphere_mesh.scale = 100.0;
  sphere_mesh.trans_vec = glm::vec3(0.0, 0.0, 0.0);

  nike_mesh.readFile("meshes/nike.off");
  nike_mesh.scale = 100.0;
  nike_mesh.trans_vec = glm::vec3(12500.0, 7500.0, 4500.0);
  nike_mesh.rotate_angles = glm::vec3(90 * DEGREE_TO_RADIAN, 0.0, 0.0);

  bear_mesh.readFile("meshes/bear.off");
  bear_mesh.scale = 100.0;
  bear_mesh.trans_vec = glm::vec3(0.0, -2500.0, 7000.0);
  bear_mesh.rotate_angles = glm::vec3(0.0, 0.0, 90 * DEGREE_TO_RADIAN);

  init();

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    update_frame_counter();
    update_light_position();
    update_view(view_mat, f);

    // GLfloat eye_x = 0.0;
    // GLfloat eye_y = -1500;
    // eye_pos = glm::vec3(eye_x, eye_y, 40000.0);
    // look_pos = glm::vec3(eye_x, eye_y, 7000.0);
    // view_mat = glm::lookAt(eye_pos, look_pos, glm::vec3(0.0, 1.0, 0.0));

    draw_ocean(vao2);
    glm::vec3 eye = eye_pos;
    eye[2] = 0.0;
    glm::vec3 pos = terrain_mesh.trans_vec;
    pos[2] = 0.0;
    if(glm::distance(pos, eye) < DETAIL_GATE) {
      draw_terrain(terrain_mesh, terrain_vao, terrain_ebo);
    } else {
      draw_terrain(terrain_mesh_s, terrain_s_vao, terrain_s_ebo);
    }

    pos = terrain_mesh2.trans_vec;
    pos[2] = 0.0;
    if(glm::distance(pos, eye) < DETAIL_GATE) {
      draw_terrain(terrain_mesh2, terrain_vao2, terrain_ebo2);
    } else {
      draw_terrain(terrain_mesh2_s, terrain_s_vao2, terrain_s_ebo2);
    }
    draw_statue(athena_mesh, athena_vao, athena_ebo);
    draw_statue(nike_mesh, nike_vao, nike_ebo);
    draw_sphere();
    draw_bear();
    draw_flock(&f, model, boid_vao, boid_idx);
    //draw_goal(&f, model, goal_vao, goal_idx);
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
