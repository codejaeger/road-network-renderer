#ifndef path_hpp
#define path_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <math.h>
#include <glm/vec2.hpp>

#include "road_network/Road-Model/road.hpp"
#include "road_network/Road-Model/road_sep.hpp"
#include "road_network/shader_util.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#define BZC 50



namespace soc{
	class Path{
		std::vector<glm::vec2> positions;
		glm::vec2 beizer_curve_positions[BZC + 1];
		GLuint vb, vao;
		GLuint shaderProgram;
		GLuint v_position, uModelViewMatrix;
		std::vector<glm::vec2> bezier_curve_point(std::vector<glm::vec2> pos, float ratio);
	  public:
	  	Path();
	  	void getPoints(GLFWwindow *window);
	  	void renderLine();
	};
}

#endif