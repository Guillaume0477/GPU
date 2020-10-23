#include "mesh.h"
#include <iostream> 

#define TINYOBJLOADER_IMPLEMENTATION
#include "external/tiny_obj_loader.h"

#include "external/glm/gtx/component_wise.hpp"
#include "external/glm/gtx/transform.hpp"

Mesh::Mesh(const std::vector<vertex>& v, const std::vector<glm::uvec3>& i)
  : vertices(v), indices(i){}

Mesh Mesh::load_from_file(const std::string filename)
{
  std::string warn, err;

  std::vector<vertex> vertices;
  std::vector<glm::uvec3> indices;

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::vector<glm::ivec3> index;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) 
  {
    std::cerr << (warn + err) << std::endl;
  }
  for (const auto& shape : shapes)
    for(const auto& indice : shape.mesh.indices)
    {
      glm::ivec3 ind = glm::ivec3(indice.vertex_index, indice.normal_index, indice.texcoord_index);
      auto it = index.begin(); 
      while ( it != index.cend() && (*it != ind) ){++it;}
      if(it == index.cend())
      {
        index.emplace_back(indice.vertex_index, indice.normal_index, indice.texcoord_index);
        glm::vec3 pos( attrib.vertices[3*indice.vertex_index],
            attrib.vertices[3*indice.vertex_index+1],
            attrib.vertices[3*indice.vertex_index+2]);

        glm::vec3 norm(1., 0., 0.);
        if(indice.normal_index != -1)
        {
          norm = glm::vec3( attrib.normals[3*indice.normal_index],
              attrib.normals[3*indice.normal_index+1],
              attrib.normals[3*indice.normal_index+2]);
        }
        glm::vec4 tang(0.,0.,0., 0.);
        glm::vec2 texcoord( attrib.texcoords[2*indice.texcoord_index],
            attrib.texcoords[2*indice.texcoord_index+1]);

        vertices.emplace_back(pos, norm, tang, texcoord);
      }
    }
  std::vector<GLuint> indices_v;
  for (const auto& shape : shapes)
    for(const auto& indice : shape.mesh.indices)
    {
      glm::ivec3 ind = glm::ivec3(indice.vertex_index, indice.normal_index, indice.texcoord_index);

      auto it = index.cbegin();
      while ( it != index.cend() && (*it != ind) ){++it;}
      if(it != index.cend())
      {
        indices_v.push_back(std::distance(index.cbegin(),it));
      }
    }
  for (auto i = 0u; i < indices_v.size(); i+=3)
    indices.emplace_back(indices_v[i], indices_v[i+1], indices_v[i+2]);

  return {vertices, indices};
}
unsigned int  Mesh::size_element() const {return 3 * indices.size();}

void Mesh::compute_normales()
{
//  for(auto i = 0u; i < vertices.size(); ++i)
//    vertices[i].normal = glm::vec3();
//
//  for(auto i = 0u; i < size_element(); ++i)
//  {
//    auto p0 = vertices[indices[i].x].position;
//    auto p1 = vertices[indices[i].y].position;
//    auto p2 = vertices[indices[i].z].position;
//
//    auto n = glm::normalize(glm::cross(p1-p0, p2-p0));
//    vertices[indices[i].x].normal = n;
//    vertices[indices[i].y].normal = n;
//    vertices[indices[i].z].normal = n;
//  }
//  for(auto i = 0u; i < vertices.size(); ++i)
//  {
//    auto& n = vertices[i].normal;
//    n = glm::normalize(n);
//  }
}

void Mesh::compute_tangant_space()
{
  compute_normales();
  for(auto i = 0u; i < vertices.size(); ++i)
    vertices[i].tangant = glm::vec4();

  std::vector<glm::vec3> tangant(vertices.size());
  std::vector<glm::vec3> bitangant(vertices.size());
  for(auto i = 0u; i < indices.size(); ++i)
  {
    vertex& v0 = vertices[indices[i].x];
    vertex& v1 = vertices[indices[i].y];
    vertex& v2 = vertices[indices[i].z];

    auto e01 = v1.position - v0.position;
    auto e02 = v2.position - v0.position;

    auto d1 = v1.texcoord - v0.texcoord;
    auto d2 = v2.texcoord - v0.texcoord;

    auto f = 1.0f / (d1.x*d2.y - d2.x*d1.y);
    auto t = f * (d2.y*e01 - d1.y * e02);
    auto b = f * (d1.x*e02 - d2.x * e01);

    tangant[indices[i].x] += t;
    tangant[indices[i].y] += t;
    tangant[indices[i].z] += t;

    bitangant[indices[i].x] += b;
    bitangant[indices[i].y] += b;
    bitangant[indices[i].z] += b;
  }
  for(auto i = 0u; i < vertices.size(); ++i)
  {
    auto& t = tangant[i];
    auto& b = bitangant[i];
    auto& n = vertices[i].normal;
    auto tortho = glm::normalize(t - glm::dot(t, n) * n);
    auto dir = (glm::dot(glm::cross(t, b), n) > 0.) ? 1. : -1.;
    vertices[i].tangant = glm::vec4( tortho, dir);
  }


}

GLuint Mesh::create_VBO() const
{
  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0].position.x, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 12 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, 12 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)(10 * sizeof(GLfloat)));
  return vbo;
}
GLuint Mesh::create_EBO() const
{
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec3), &indices[0].x, GL_STATIC_DRAW);
  return ebo;
}
GLuint Mesh::load_to_gpu() const
{
  GLuint vao, vbo, ebo;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  vbo = create_VBO();
  ebo = create_EBO();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return vao;
}

Mesh Mesh::create_grid(int N)
{
  std::vector<vertex> vertices;
  std::vector<glm::uvec3> indices;

  GLfloat delta = 1./(float(N)-1.);

  for(int i = 0; i < N; ++i)
  {
    for(int j = 0; j < N; ++j)
    {
      glm::vec3 p(-1.+2.*j*delta, 0.,-1.+2.*i*delta);
      glm::vec3 n(0., 1., 0.);
      glm::vec4 t(1., 0., 0., 1.);
      glm::vec2 ct(j*delta, i*delta);
      vertices.emplace_back(p, n, t, ct);
    }
  }
  for(int i = 0; i < N*(N-1); ++i)
  {
    if( (i+1)%N != 0 )
    {
      indices.emplace_back( i+1,i,  i+N);
      indices.emplace_back(i+1, i+N, i+ N +1);
    }
  }
  return {vertices, indices};
}
Mesh Mesh::create_sphere(int N)
{
  std::vector<vertex> vertices;
  std::vector<glm::uvec3> indices;

  GLfloat delta = 1./(float(N)-1.);
  float pi = 3.14159265358979;
  float delta_u = pi * 2. * delta;
  float delta_v = pi * delta;

  float u = -pi;
  for(int i = 0; i < N; ++i)
  {
  float v = -pi/2.;
    for(int j = 0; j < N; ++j)
    {
      glm::vec3 p(cos(u)*cos(v), sin(u)*cos(v),sin(v));
      glm::vec3 n(p);
      glm::vec4 t(0., 0., 0., 0.);
      glm::vec2 ct(j*delta, i*delta);
      vertices.emplace_back(p, n, t, ct);

      v += delta_v;
    }
      u += delta_u;
  }
  for(int i = 0; i < N*(N-1); ++i)
  {
    if( (i+1)%N != 0 )
    {
      indices.emplace_back(i, i+1, i+N);
      indices.emplace_back(i+1, i+N, i+ N +1);
    }
  }
  Mesh m = {vertices, indices};
  m.compute_tangant_space();

  return m;
}
void Mesh::apply_matrix(const glm::mat4& m)
{
  for (auto i = 0u; i < vertices.size(); ++i)
  {
    glm::vec4 p(vertices[i].position, 1.);
    glm::vec4 n(vertices[i].normal, 0.);
    //glm::vec4 t(vertices[i].tangant, 0.);
    p = m * p;
    n = m * n;
    //t = m * t;
    vertices[i].position = p;
    vertices[i].normal = n;
    //vertices[i].tangant = t;
  }
}

void Mesh::normalize()
{
  auto min = glm::vec3(std::numeric_limits<float>::max());
  auto max = glm::vec3(std::numeric_limits<float>::lowest());

  for (auto i = 0u; i < vertices.size(); ++i) {
    glm::vec3 p(vertices[i].position);
    min = glm::min(p, min);
    max = glm::max(p, max);
  }
  auto center = (min + max) * 0.5f;
  auto d = max - min;
  auto scale = 1. / glm::compMax(d);
  glm::mat4 m =
    glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(scale)), -center);
  apply_matrix(m);
}
