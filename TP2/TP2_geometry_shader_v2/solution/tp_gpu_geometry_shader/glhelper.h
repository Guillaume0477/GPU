#ifndef GLHELPER_H
#define GLHELPER_H

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <fstream>
#include <vector>

namespace glhelper
{
  // return id of the gpu program
  GLuint create_program_from_file(const std::string& vs_file,
      const std::string& fs_file);


// -- Ceci est un element de correction
  // return id of the gpu program
  GLuint create_program_from_file(const std::string& vs_file,
      const std::string& gs_file,
      const std::string& fs_file);
// -- Fin element de correction


  // return id of the gpu program
  GLuint create_program(const std::string& vs_content,
      const std::string& fs_content);


// -- Ceci est un element de correction
  // return id of the gpu program
  GLuint create_program(const std::string& vs_content,
      const std::string& gs_content,
      const std::string& fs_content);
// -- Fin element de correction


  // return id of the compiled shader
  GLuint compile_shader(const char* shader_content, GLenum shader_type);

  // return content of file filename
  std::string read_file(const std::string& filename);

  // Creation texture et envoie sur GPU
  GLuint load_texture(std::string filename);

  // Creation d'une image
  void load_image(const std::string& filename,
      unsigned char*& image,
      int& w,
      int& h,
      bool& rgb);

  void print_screen(int w, int h, std::string filename = "");

} // namespace glhelper

#endif
