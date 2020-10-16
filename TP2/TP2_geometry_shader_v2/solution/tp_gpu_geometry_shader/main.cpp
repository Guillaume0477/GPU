#include <iostream>
#include <fstream>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glhelper.h"
#include "mesh.h"
#include "camera.h"

#include "external/glm/gtc/matrix_transform.hpp"


GLuint VAO;
GLuint n_elements;

// camera
Camera cam;


// -- Ceci est un element de correction
std::vector<GLuint> program_ids;
unsigned int current_program = 0;
bool text_norm = false;
// -- Fin element de correction



void init()
{

    glClearColor(0.1, 0.1, 0.1, 0);
    glEnable(GL_DEPTH_TEST);


// -- Ceci est un element de correction
    program_ids.push_back(glhelper::create_program_from_file("basic.vs", "basic.fs"));
    program_ids.push_back(glhelper::create_program_from_file("basic.vs", "correction/textures.fs"));
    program_ids.push_back(glhelper::create_program_from_file("basic.vs", "correction/computenormales.gs","correction/normalcolor.fs"));
    program_ids.push_back(glhelper::create_program_from_file("basic.vs", "correction/shownormales.gs","correction/basicred.fs"));
    program_ids.push_back(glhelper::create_program_from_file("basic.vs", "correction/showtriangles.gs","correction/basicred.fs"));
    program_ids.push_back(glhelper::create_program_from_file("basic.vs", "correction/explode.gs","correction/textures_after_geom.fs"));
    program_ids.push_back(glhelper::create_program_from_file("basic.vs", "correction/inflate.gs","correction/textures_after_geom.fs"));
    program_ids.push_back(glhelper::create_program_from_file("basic.vs", "correction/culling.gs","correction/textures_after_geom.fs"));
// -- Fin element de correction


    Mesh m = Mesh::load_from_file("data/Frankie/Frankie.obj");
    m.compute_normales();
    n_elements= m.size_element();
    VAO = m.load_to_gpu();

    glhelper::load_texture("data/Frankie/flyingsquirrel_skin_col.png");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


// -- Ceci est un element de correction
    if(text_norm)
    {
      glUseProgram(program_ids[3]);
      glBindVertexArray(VAO);
      set_uniform_mvp(program_ids[3]);
      glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);
    }
//    glUseProgram(program_line_id);
//    glBindVertexArray(VAO);
//    set_uniform_mvp(program_line_id);
//    glLineWidth(3.);
//    glDrawElements(GL_LINES, n_elements, GL_UNSIGNED_INT, 0);
//
//    glUseProgram(program_geom_id);
//    glBindVertexArray(VAO);
//    set_uniform_mvp(program_id);
//    glLineWidth(3.);
//    glDrawElements(GL_LINES, n_elements, GL_UNSIGNED_INT, 0);
//    glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);

    glUseProgram(program_ids[current_program]);
    glBindVertexArray(VAO);
    set_uniform_mvp(program_ids[current_program]);
    glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);

// -- Fin element de correction



    glBindVertexArray(0);
    glutSwapBuffers ();
}

static void keyboard_callback(unsigned char key, int, int)
{
  switch (key)
  {

// -- Ceci est un element de correction
    case 'n':
      current_program = ++current_program % program_ids.size();
      break;
    case '1':
        text_norm = !text_norm;
      break;
// -- Fin element de correction

    case 'p':
      glhelper::print_screen(cam.width(), cam.height());
      break;
    case 'q':
    case 'Q':
    case 27:
      exit(0);
  }
  glutPostRedisplay();
}

static void reshape_callback(int width, int height)
{
  cam.common_reshape(width,height);

  glViewport(0,0, width, height);
  glutPostRedisplay();
}
static void mouse_callback (int button, int action, int x, int y)
{
  cam.common_mouse(button, action, x, y);
  glutPostRedisplay();
}

static void motion_callback(int x, int y)
{
  cam.common_motion(x, y);
  glutPostRedisplay();
}

static void timer_callback(int)
{
  glutTimerFunc(25, timer_callback, 0);
  glutPostRedisplay();
}
int main(int argc, char** argv)
{
  glutInitContextVersion(3, 3);
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

  init();
  std::cout << cam.position()[0] << " " << cam.position()[1] << " " << cam.position()[2] << std::endl;
  glutMainLoop();

  return 0;
}
