#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

class Shader
{
public:
    typedef std::vector<std::string> DefineList;

    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    Shader(const std::string &vertexPath, const std::string &fragmentPath, const DefineList &defines);

    void uniform(const std::string &name, bool v);
    void uniform(const std::string &name, GLint v);
    void uniform(const std::string &name, GLfloat v);
    void uniform(const std::string &name, glm::vec2 v);
    void uniform(const std::string &name, glm::vec3 v);
    void uniform(const std::string &name, glm::vec4 v);
    void uniform(const std::string &name, glm::mat3 m);
    void uniform(const std::string &name, glm::mat4 m);

    void uniform(const std::string &name, const std::vector<bool> &v);
    void uniform(const std::string &name, const std::vector<GLint> &v);
    void uniform(const std::string &name, const std::vector<GLfloat> &v);
    void uniform(const std::string &name, const std::vector<glm::vec2> &v);
    void uniform(const std::string &name, const std::vector<glm::vec3> &v);
    void uniform(const std::string &name, const std::vector<glm::vec4> &v);
    void uniform(const std::string &name, const std::vector<glm::mat3> &v);
    void uniform(const std::string &name, const std::vector<glm::mat4> &v);

    void use();

private:
    static GLuint compileAndLink(const std::string &vertexPath, const std::string &fragmentPath, const DefineList &defines);
    static std::string load(const std::string &path, const DefineList &defines);
    static std::string load(const std::string &path, unsigned int level = 0);
    static std::string preprocess(const std::string &line, const std::string &directory, unsigned int level);

    GLint GetUniformLocation(const std::string &name);

    GLuint m_program;

    typedef std::map<std::string, GLint> UniformLUTMap;
    UniformLUTMap uniformLUT;
};

typedef std::shared_ptr<Shader> ShaderPtr;
