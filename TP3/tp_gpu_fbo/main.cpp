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

// Frame buffer to use
GLuint frame_buffer;
GLuint tex_frame_buf;
GLuint render_buffer;
unsigned char* im_text_fb;

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

    //Creation of a frame buffer object
    glGenFramebuffers(1,&frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

    glGenTextures(1, &tex_frame_buf);
    glBindTexture(GL_TEXTURE_2D, tex_frame_buf);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cam.width(), cam.height() , 0, GL_RGB, GL_UNSIGNED_BYTE, im_text_fb);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);    

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_frame_buf, 0);

    glGenRenderbuffers(1, &render_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, cam.width(), cam.height());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cout << "PB FRAME BUFFER" << std::endl;
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

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Affichage de l'écureil
    glUseProgram(program_id);
    glBindVertexArray(VAO);
    set_uniform_mvp(program_id);
    glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);

    glhelper::print_screen(cam.width(), cam.height(), "firstAttempt.png");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, tex_frame_buf);
    // // Affichage du sol
    glUseProgram(program_id);
    glBindVertexArray(VAO_wall);
    set_uniform_mvp(program_id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(program_id);
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
