#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <string>
#include <vector>

#include "external/glm/glm.hpp"
    struct vertex{
      glm::vec3 position, normal;
      glm::vec4 tangant; // last component store orientation
      glm::vec2 texcoord;
      vertex(glm::vec3 p, glm::vec3 n, glm::vec4 t, glm::vec2 tc)
        : position(p), normal(n), tangant(t), texcoord(tc){}
    };
class Mesh
{
  public:

    /***************************************************/
    //         Creation of a mesh
    /***************************************************/
    Mesh() = default;
    // Create a mesh with vertices containing 3D point, 3D normales and 2D uv coordinates
    Mesh(const std::vector<vertex>& vertices, const std::vector<glm::uvec3>& indices);
    // Create a mesh according to an obj file
    static Mesh load_from_file(const std::string filename);
    // Create a mesh on a grid on xz plane between -1:1 with NxN elements
    static Mesh create_grid(int N);
    static Mesh create_sphere(int N);

    void compute_normales();
    void compute_tangant_space();

    /***************************************************/
    //         Mesh to openGL
    /***************************************************/
    // Load the mesh to a vao, return vao id
    GLuint load_to_gpu() const;
    // Load the mesh to a vbo, return vbo id
    GLuint create_VBO() const;
    // Load the mesh to a ebo, return ebo id
    GLuint create_EBO() const;

    /***************************************************/
    //         Retrieve mesh informations
    /***************************************************/
    unsigned int size_element() const;

    /***************************************************/
    //         Modification  mesh
    /***************************************************/
    void apply_matrix(const glm::mat4& m);
    void normalize();

  private:

    std::vector<vertex> vertices;
    std::vector<glm::uvec3> indices;
};

#endif
