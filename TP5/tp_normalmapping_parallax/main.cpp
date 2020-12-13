#include <iostream>
#include <fstream>
#include <chrono>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glhelper.h"
#include "mesh.h"
#include "camera.h"

// main obj
GLuint program_id;
GLuint VAO;
GLuint n_elements;

// camera
Camera cam;

void init()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

  program_id = glhelper::create_program_from_file("./shaders/simple.vs", "./shaders/simple.fs");
  glUseProgram(program_id);

  Mesh m = Mesh::create_grid(100);
  //  Mesh m = Mesh::create_sphere(100);
  //  Mesh m = Mesh::load_from_file("./data/Frankie/Frankie.obj");
  //  m.apply_matrix(glm::mat4(1.f,0.f,0.f,0.f,  0.f,-1.f,0.f,0.f, 0.f,0.f,1.f,0.f, 0.f,0.f,0.f,1.f));
  m.compute_tangant_space();
  n_elements = m.size_element();
  VAO = m.load_to_gpu();  CHECK_GL_ERROR();

  glUseProgram(program_id);

  glActiveTexture(GL_TEXTURE0);
  //GLuint tex0 = glhelper::load_texture("./data/Height/pilou.png");//"./data/bricks2.jpg"); //
  //GLuint tex0 = glhelper::load_texture("./data/Height/pilouTrue.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/access.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/access_Blur.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/access_Smooth.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/avslope.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/avslope_Blur.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/avslope_Smooth.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/gradient.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/gradient_Blur.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/gradient_Smooth.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/hauteur.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/hauteur_Blur.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/hauteur_Smooth.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/hauteur_phong.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/hauteur_phong_Blur.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/hauteur_phong_Smooth.png");
  GLuint tex0 = glhelper::load_texture("./data/Height/lapla.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/lapla_Blur.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/lapla_Smooth.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/slope.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/slope_Blur.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/slope_Smooth.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/StreamArea.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/StreamArea_Blur.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/StreamArea_Smooth.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/StreamAreaStreepest.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/StreamAreaStreepest_Blur.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/StreamAreaStreepest_Smooth.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/StreamPower.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/StreamPower_Blur.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/StreamPower_Smooth.png");








  //GLuint tex0 = glhelper::load_texture("./data/Height/lapla.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/pilou_smooth.png");//"./data/bricks2.jpg"); //
  //GLuint tex0 = glhelper::load_texture("./data/Height/pilouTrue_smooth.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/grad_smooth.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/lapla_smooth.png");
  //GLuint tex0 = glhelper::load_texture("./data/Height/StreamAreaStreepest_Smooth.png"); 
  //GLuint tex0 = glhelper::load_texture("./data/Height/StreamPower_Smooth.png"); 


  GLuint location = glGetUniformLocation(program_id, "textureSampler");
  glUniform1i(location, 0);

  glActiveTexture(GL_TEXTURE0 + 1); 
  GLuint tex1 = glhelper::load_texture("./data/Height/heightmap3.png");//"./data/bricks2_disp.jpg"); //
  GLuint location1 = glGetUniformLocation(program_id, "textureSampler1");
  glUniform1i(location1, 1);

  glActiveTexture(GL_TEXTURE0 + 2);
  GLuint tex2 = glhelper::load_texture("./data/Rocks002_2K/Rocks002_2K_Normal.png");//"./data/bricks2_normal.jpg"); //
  GLuint location2 = glGetUniformLocation(program_id, "textureNormals");
  glUniform1i(location2, 2);

  glActiveTexture(GL_TEXTURE0 + 3);
  GLuint tex3 = glhelper::load_texture("./data/Rocks002_2K/Rocks002_2K_AmbientOcclusion.png");
  GLuint location3 = glGetUniformLocation(program_id, "textureAO");
  glUniform1i(location3, 3);


}

void set_uniform_mvp(GLuint program)
{
  GLint mvp_id = glGetUniformLocation(program, "MVP");
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 mvp = cam.projection()*cam.view()*model;
  if (mvp_id != -1)
  {
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
  }
  else{ std::cerr << "Couldn't set MVP \n" << mvp << std::endl;}
}
void set_uniform_camera_pos(GLuint program)
{
  GLint cam_pos_id = glGetUniformLocation(program, "camera");
  const auto& p = cam.position();
  if (cam_pos_id != -1)
  {
    glUniform3f(cam_pos_id, p.x, p.y, p.z);
  }
  else{ std::cerr << "Couldn't set camera position " << p << std::endl;}
}
void set_uniform_model(GLuint program)
{
  GLint model_id = glGetUniformLocation(program, "model");
  glm::mat4 model = glm::mat4(1.0f);
  if (model_id != -1)
  {
    glUniformMatrix4fv(model_id, 1, GL_FALSE, &model[0][0]);
  }
  else{ std::cerr << "Couldn't set model \n" << model << std::endl;}
}
static void display_callback()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program_id);
  glBindVertexArray(VAO);

  set_uniform_camera_pos(program_id);
  set_uniform_mvp(program_id);
  set_uniform_model(program_id);

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
  glutPostRedisplay();
  glutTimerFunc(25, timer_callback, 0);
}

int main(int argc, char** argv)
{
  glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(cam.width(), cam.height());
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
