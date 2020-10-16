#include <iostream>
#include <fstream>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glhelper.h"
#include "mesh.h"
#include "camera.h"

#include "external/glm/gtc/matrix_transform.hpp"


std::vector<GLuint> Programmes;
GLuint program_id, program_id_2;
std::vector<std::vector<float>> color_array;
int indice_couleur = 0;
int id_prog = 0;

GLuint VAO;
GLuint n_elements;

// camera
Camera cam;

void init()
{

    glClearColor(0.1, 0.1, 0.1, 0);
    glEnable(GL_DEPTH_TEST);

    program_id = glhelper::create_program_from_file("basic.vs", "basic.fs");
    program_id_2 = glhelper::create_program_from_file("basic.vs", "color.fs");
    
    Programmes.push_back(program_id);
    Programmes.push_back(program_id_2);
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

    GLuint location = glGetUniformLocation(program_id_2, "color_1");
    glUniform3fv( location,1, &color_array[indice_couleur][0]);

    glUseProgram(Programmes[id_prog]);
    glBindVertexArray(VAO);
    set_uniform_mvp(program_id);
    glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);




    glBindVertexArray(0);
    glutSwapBuffers ();
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
    case 'd':
      if (indice_couleur < int(color_array.size()-1)){
        indice_couleur = indice_couleur + 1 ;
      }
      break;
    case 's':
      if (indice_couleur != 0){
        indice_couleur = indice_couleur - 1 ;
      }
      break;
    case 'm':
      if (id_prog+1 < Programmes.size()){
        id_prog++;
      }
      glUseProgram(Programmes[id_prog]);
      break;
    case 'l':
      if (id_prog-1 >= 0){
        id_prog--;
      }
      glUseProgram(Programmes[id_prog]);
      break;
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
  color_array.push_back({0,0,1});
  color_array.push_back({0,1,0});
  color_array.push_back({1,0,0});
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
