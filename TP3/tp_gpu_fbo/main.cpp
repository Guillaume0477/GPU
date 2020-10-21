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
GLuint texture;

GLuint VAO_wall;

// camera
Camera cam;

// Current program id
GLuint program_id;

//framebuffer
GLuint framebuffer;
GLuint texColorBuffer;
GLuint rboDepthStencil;


void init()
{

    glClearColor(0.1, 0.1, 0.1, 0);
    glEnable(GL_DEPTH_TEST);

    program_id = glhelper::create_program_from_file("basic.vs", "basic.fs");

    Mesh m = Mesh::load_from_file("data/Frankie/Frankie.obj");
    m.compute_normales();
    n_elements= m.size_element();
    VAO = m.load_to_gpu();

    texture = glhelper::load_texture("data/Frankie/flyingsquirrel_skin_col.png");

    Mesh m_wall = Mesh::create_grid(2);
    m_wall.apply_matrix(glm::translate(glm::mat4(1.), glm::vec3(0.f,0.35f,0.f)));
    VAO_wall = m_wall.load_to_gpu();

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer,0);


    glGenRenderbuffers(1, &rboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);

    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);

    std::cout<<glCheckFramebufferStatus(GL_FRAMEBUFFER)<<GL_FRAMEBUFFER_COMPLETE<<std::endl;
    
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

    // Bind our framebuffer and draw 3D scene (spinning cube)
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindVertexArray(VAO);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(program_id);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);  
    
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Affichage du sol

    glUseProgram(program_id);
    glBindVertexArray(VAO_wall);
    set_uniform_mvp(program_id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Affichage de l'écureil
    glUseProgram(program_id);
    glBindVertexArray(VAO);
    set_uniform_mvp(program_id);
    glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);

    // Bind default framebuffer and draw contents of our framebuffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //glViewport(0, 0, window_width(), window_height());
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glBlitFramebuffer(
        0, 0, 800, 600, // rectangle origine dans READ_FRAMEBUFFER
        0, 0, 800, 600, // rectangle destination dans DRAW_FRAMEBUFFER
        GL_COLOR_BUFFER_BIT, GL_LINEAR);     


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
  glutMainLoop();

  return 0;
}
