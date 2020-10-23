#include <chrono>
#include <iostream>
#include <fstream>
#include <random>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glhelper.h"
#include "camera.h"

const int NB_PARTICULES = 100000;

GLuint VAO;
GLuint VBO[2];

// camera
Camera cam;

// Current program id
GLuint program_id;
GLuint cs_id, cs_program_id;

int nframe;
std::chrono::time_point<std::chrono::high_resolution_clock> start;

void init()
{
  program_id = glhelper::create_program_from_file("basic.vs", "basic.fs");

  cs_id = glhelper::compile_shader(glhelper::read_file("cs.vs").c_str(), GL_COMPUTE_SHADER);
  cs_program_id = glCreateProgram();
  glAttachShader(cs_program_id, cs_id);
  glLinkProgram(cs_program_id);
  glhelper::check_error_link(cs_program_id);
  glDeleteShader(cs_id);

  start = std::chrono::high_resolution_clock::now();


  GLfloat positions[NB_PARTICULES*3];
  GLfloat vitesses[NB_PARTICULES*3];

  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(-0.3,0.3);
  for(auto i = 0u; i < NB_PARTICULES*3; ++i)
  {
    positions[i] = 0.;
    vitesses[i] = ((i+2)%3) == 0 ? std::fabs(distribution(generator)) : distribution(generator);
  }

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(2, VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vitesses), vitesses, GL_DYNAMIC_DRAW);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glPointSize(3);

}

void compute_fps()
{
 ++nframe;
  if(nframe == 100)
  {
    auto stop = std::chrono::high_resolution_clock::now();
    auto diff = (stop-start).count();
    start = stop;
    std::cout << 100./(float(diff) / 1e9) << std::endl;
    nframe = 0;
  }
}

void set_uniform_mvp(GLuint program)
{
  GLint mvp_id = glGetUniformLocation(program, "MVP");
  if (mvp_id != -1)
  {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = cam.projection()*cam.view()*model;
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
  }
}

static void display_callback()
{
  
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(cs_program_id);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO[0]);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, VBO[0]);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO[1]);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, VBO[1]);
  glDispatchCompute(NB_PARTICULES/10, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  std::vector<float> buff(3*NB_PARTICULES); // n is the size
  glGetNamedBufferSubData( VBO[0] ,0 , 3* NB_PARTICULES * sizeof(float), buff.data());

  std::cout << buff[0] << ' ' << buff[1] << ' ' << buff[2] << std::endl;

  glUseProgram(program_id);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0,0);
  glEnableVertexAttribArray(0);

  set_uniform_mvp(program_id);

  glDrawArrays(GL_POINTS, 0, NB_PARTICULES);

  glutSwapBuffers();

  compute_fps();
  //only needed for benchmark
  //glutPostRedisplay();
}

static void keyboard_callback(unsigned char key, int, int)
{
  switch (key)
  {
    case 'p':
      glhelper::print_screen(cam.width(), cam.height());
      break;
    case 'q':
    case 'Q':
    case 27:
      exit(0);
  }
}

static void reshape_callback(int width, int height)
{
  cam.common_reshape(width,height);

  glViewport(0,0, width, height);
}
static void mouse_callback (int button, int action, int x, int y)
{
  cam.common_mouse(button, action, x, y);
}

static void motion_callback(int x, int y)
{
  cam.common_motion(x, y);
}

static void timer_callback(int)
{
  glutTimerFunc(25, timer_callback, 0);
  glutPostRedisplay();
}

// From : https://zestedesavoir.com/tutoriels/1554/introduction-aux-compute-shaders/
void printWorkGroupsCapabilities() {
  int workgroup_count[3];
  int workgroup_size[3];
  int workgroup_invocations;

  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workgroup_count[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workgroup_count[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workgroup_count[2]);

  printf ("Taille maximale des workgroups:\n\tx:%u\n\ty:%u\n\tz:%u\n",
      workgroup_size[0], workgroup_size[1], workgroup_size[2]);

  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workgroup_size[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workgroup_size[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workgroup_size[2]);

  printf ("Nombre maximal d'invocation locale:\n\tx:%u\n\ty:%u\n\tz:%u\n",
      workgroup_size[0], workgroup_size[1], workgroup_size[2]);

  glGetIntegerv (GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroup_invocations);
  printf ("Nombre maximum d'invocation de workgroups:\n\t%u\n", workgroup_invocations);
}

int main(int argc, char** argv)
{
  glutInitContextVersion(4, 3);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(cam.height(), cam.width());
  glutCreateWindow("opengl");
  glutDisplayFunc(display_callback);
  glutMotionFunc(motion_callback);
  glutMouseFunc(mouse_callback);
  glutKeyboardFunc(keyboard_callback);
  glutReshapeFunc(reshape_callback);
  glutTimerFunc(25, timer_callback, 0);

  glewExperimental=true;
  glewInit();

  printWorkGroupsCapabilities();
  init();
  glutMainLoop();

  return 0;
}
