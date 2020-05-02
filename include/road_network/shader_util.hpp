#ifndef shader_util_hpp
#define shader_util_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace soc {

GLuint LoadShaderGL(GLenum eShaderType, const std::string &strFilename);
GLuint CreateShaderGL(GLenum eShaderType, const std::string &strShaderFile);
GLuint CreateProgramGL(const std::vector<GLuint> &shaderList);

} // End namespace soc

#endif /* shader_util_hpp */
