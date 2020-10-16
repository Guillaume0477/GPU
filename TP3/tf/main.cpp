#include <iostream>
#include <fstream>
#include <chrono>


#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glhelper.h"
#include "mesh.h"
#include "camera.h"

#define TINYOBJLOADER_IMPLEMENTATION 
#include "external/tiny_obj_loader.h"

// main obj
GLuint program_id;
GLuint VAO;
GLuint query;
GLuint n_elements;
GLuint n_points;
GLuint texture_id;
GLuint program_tf_id;
GLuint VBO[4]; // 0 and 1 are for the position 2 for the normal and 3 for the texcoor

// camera
Camera cam;

void init()
{
  // create main obj
  {
    std::cout << "Creation of main object" << std::endl; 
    program_id = glhelper::create_program_from_file("shaders/simple.vs", "shaders/simple.fs");
    Mesh m = Mesh::load_from_file("data/Frankie/Frankie3.obj");
    n_elements=m.size_element();
    auto pos = m.position();
    auto normal = m.normal();
    auto texcoord = m.texcoord();
    n_points = pos.size()/3;

    texture_id = glhelper::load_texture("data/Frankie/flyingsquirrel_skin_col.png");

    // create transform feedback
    auto content =  glhelper::read_file("shaders/tf.vs");
    auto shader_id = glhelper::compile_shader(content.c_str(), GL_VERTEX_SHADER);
    program_tf_id = glCreateProgram();
    glAttachShader(program_tf_id, shader_id);

    const GLchar* attributes[] = {"pos"};
    glTransformFeedbackVaryings(program_tf_id, 1, attributes, GL_SEPARATE_ATTRIBS);
    glLinkProgram(program_tf_id);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.idx().size() * sizeof(GLuint), m.idx().data(), GL_STATIC_DRAW);

    glGenBuffers(4, VBO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(GLfloat), pos.data(), GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(GLfloat), pos.data(), GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(GLfloat), normal.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, texcoord.size() * sizeof(GLfloat), texcoord.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenQueries(1, &query);
  }

  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glEnable(GL_DEPTH_TEST);            
}

static void display_callback()
{
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

  glm::mat4 mvp = cam.projection() * cam.view();

  // displacement with tf
  {	
    glEnable(GL_RASTERIZER_DISCARD);
    glUseProgram(program_tf_id);
    glBindVertexArray(VAO);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, VBO[1]);
    glBindBuffer(GL_ARRAY_BUFFER,  VBO[0]);
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,0,0);
    //glBeginTransformFeedback(GL_TRIANGLES);
    //glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);
    //glEndTransformFeedback();
    glEnableVertexAttribArray(1);                                                 
    glBeginTransformFeedback(GL_POINTS);
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);                
    glDrawArrays(GL_POINTS, 0, n_points);
    glEndTransformFeedback();
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);                         
    GLuint n;
    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &n);              
    glFlush();
    glDisableVertexAttribArray(1);                                                
    glDisable(GL_RASTERIZER_DISCARD);
    std::swap(VBO[0], VBO[1]);
  }

  // display obj
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, cam.width(), cam.height());
    glUseProgram(program_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLint mvp_id = glGetUniformLocation(program_id, "MVP");
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]); 
    glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0); 
  }

  glBindVertexArray(0);
  glutSwapBuffers ();
}

static void keyboard_callback(unsigned char key, int, int)
{
  switch (key)
  {
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
