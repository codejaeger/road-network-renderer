#ifndef SKYMAPS_HPP
#define SKYMAPS_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "road_network/shader_util.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include "road_network/Road-Model/road.hpp"
extern std::vector<glm::mat4> matrixStack;

namespace soc {
	class SkyMaps {
		GLuint loadTexture(const char *path);
		GLuint loadCubemap(std::vector<std::string> faces);
		GLuint skyboxShader;
		GLuint skyboxVAO, skyboxVBO;
		GLuint cubemapTexture;
	public:
		SkyMaps();
		~SkyMaps();
		void render();
	};
}

#endif