#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

Shader::Shader(const string &vertexPath, const string &fragmentPath) : m_program(compileAndLink(vertexPath, fragmentPath, Shader::DefineList())) { }

Shader::Shader(const string &vertexPath, const string &fragmentPath, const Shader::DefineList &defines) : m_program(compileAndLink(vertexPath, fragmentPath, defines)) { }

void Shader::uniform(const string &name, bool v)
{
  GLint loc = GetUniformLocation(name);
  glUniform1i(loc, v ? 1 : 0);
}

void Shader::uniform(const string &name, GLint v)
{
  GLint loc = GetUniformLocation(name);
  glUniform1i(loc, v);
}

void Shader::uniform(const string &name, GLfloat v)
{
  GLint loc = GetUniformLocation(name);
  glUniform1f(loc, v);
}

void Shader::uniform(const string &name, vec2 v)
{
  GLint loc = GetUniformLocation(name);
  glUniform2f(loc, v[0], v[1]);
}

void Shader::uniform(const string &name, vec3 v)
{
  GLint loc = GetUniformLocation(name);
  glUniform3f(loc, v[0], v[1], v[2]);
}

void Shader::uniform(const string &name, vec4 v)
{
  GLint loc = GetUniformLocation(name);
  glUniform4f(loc, v[0], v[1], v[2], v[3]);
}

void Shader::uniform(const string &name, mat3 m)
{
  GLint loc = GetUniformLocation(name);
  glUniformMatrix3fv(loc, 1, GL_FALSE, value_ptr(m));
}

void Shader::uniform(const string &name, mat4 m)
{
  GLint loc = GetUniformLocation(name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(m));
}

void Shader::uniform(const string &name, const vector<bool> &v)
{
    GLint loc = GetUniformLocation(name);
    for (unsigned int i = 0; i < v.size(); i++) {
        uniform(name + "[" + to_string(i) + "]", v[i]);
    }
}

void Shader::uniform(const string &name, const vector<GLint> &v)
{
    GLint loc = GetUniformLocation(name);
    for (unsigned int i = 0; i < v.size(); i++) {
        uniform(name + "[" + to_string(i) + "]", v[i]);
    }
}

void Shader::uniform(const string &name, const vector<GLfloat> &v)
{
    GLint loc = GetUniformLocation(name);
    for (unsigned int i = 0; i < v.size(); i++) {
        uniform(name + "[" + to_string(i) + "]", v[i]);
    }
}

void Shader::uniform(const string &name, const vector<glm::vec2> &v)
{
    GLint loc = GetUniformLocation(name);
    for (unsigned int i = 0; i < v.size(); i++) {
        uniform(name + "[" + to_string(i) + "]", v[i]);
    }
}

void Shader::uniform(const string &name, const vector<glm::vec3> &v)
{
    GLint loc = GetUniformLocation(name);
    for (unsigned int i = 0; i < v.size(); i++) {
        uniform(name + "[" + to_string(i) + "]", v[i]);
    }
}

void Shader::uniform(const string &name, const vector<glm::vec4> &v)
{
    GLint loc = GetUniformLocation(name);
    for (unsigned int i = 0; i < v.size(); i++) {
        uniform(name + "[" + to_string(i) + "]", v[i]);
    }
}

void Shader::uniform(const string &name, const vector<glm::mat3> &v)
{
    GLint loc = GetUniformLocation(name);
    for (unsigned int i = 0; i < v.size(); i++) {
        uniform(name + "[" + to_string(i) + "]", v[i]);
    }
}

void Shader::uniform(const string &name, const vector<glm::mat4> &v)
{
    GLint loc = GetUniformLocation(name);
    for (unsigned int i = 0; i < v.size(); i++) {
        uniform(name + "[" + to_string(i) + "]", v[i]);
    }
}

void Shader::use()
{
    glUseProgram(this->m_program);
}

//#define SHADER_DEBUG

GLuint Shader::compileAndLink(const string &vertexPath, const string &fragmentPath, const Shader::DefineList &defines) {
    string vertexCode = load(vertexPath, defines);
    string fragmentCode = load(fragmentPath, defines);

#ifdef SHADER_DEBUG
    cout << "<vs path=" << vertexPath << ">\n" << vertexCode << "</vs>" << endl;
    cout << "<fs path=" << fragmentPath << ">\n" << fragmentCode << "</fs>" << endl;
#endif

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar * fShaderCode = fragmentCode.c_str();
    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }
    // Shader Program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    // Print linking errors if any
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

string Shader::load(const string &path, const Shader::DefineList &defines)
{
    cout << "Loading " << path << endl;

    stringstream shaderStream;
    string directory = path.substr(0, path.find_last_of('/'));

    try
    {
        ifstream shaderFile;
        shaderFile.exceptions (ifstream::badbit);
        shaderFile.open(path);

        string line;
        unsigned int lineNumber = 0;
        while (getline(shaderFile, line)) {
            if (lineNumber == 1) {
                for (Shader::DefineList::const_iterator itr = defines.begin(); itr != defines.end(); itr++) {
                    shaderStream << "#define " << *itr << endl;
                }
            }

            shaderStream << preprocess(line, directory, 1) << endl;
            lineNumber++;
        }
    }
    catch (ifstream::failure e)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }

    return shaderStream.str();
}

string Shader::load(const string &path, unsigned int level)
{
    if (level > 32) {
        throw "Header inclusion depth limit reached, might be caused by cyclic header inclusion";
    }

    cout << "Loading " << path << endl;

    stringstream shaderStream;
    string directory = path.substr(0, path.find_last_of('/'));

    try
    {
        ifstream shaderFile;
        shaderFile.exceptions (ifstream::badbit);
        shaderFile.open(path);

        string line;
        while (getline(shaderFile, line)) {
            shaderStream << preprocess(line, directory, level + 1) << endl;
        }
    }
    catch (ifstream::failure e)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }

    return shaderStream.str();
}

string Shader::preprocess(const string &line, const string &directory, unsigned int level)
{
    static const regex re("^[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
    smatch matches;

    if (regex_search(line, matches, re)) {
        string path = matches[1].str();

        return load(directory + "/" + path, level);
    } else {
        return line;
    }
}

GLint Shader::GetUniformLocation(const string &name)
{
#ifdef SHADER_NEVER_CACHE_UNIFORM_LOCATION
    return glGetUniformLocation(m_program, name.c_str());
#endif

    UniformLUTMap::iterator itr = uniformLUT.find(name);

    if (itr == uniformLUT.end())
    {
        GLint loc = glGetUniformLocation(m_program, name.c_str());
        cout << "Looking up uniform " << name << " with location=" << loc << endl;

        uniformLUT[name] = loc;
        return loc;
    }

    return itr->second;
}

