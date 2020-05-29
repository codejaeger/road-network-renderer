#include "road_network/SkyMaps/skymaps.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "road_network/stb_image.h"
namespace soc {
	SkyMaps::SkyMaps() {
	  std::string vertex_shader_file("./src/SkyMaps/vertex-shaders/skybox.vs");
  	std::string fragment_shader_file("./src/SkyMaps/fragment-shaders/skybox.fs");

	  std::vector<GLuint> shaderList;
	  shaderList.push_back(soc::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
	  shaderList.push_back(soc::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

	  skyboxShader = soc::CreateProgramGL(shaderList);

    GLfloat skyboxVertices[] = {
      // positions          
      -5.0f,  5.0f, -5.0f,
      -5.0f, -5.0f, -5.0f,
       5.0f, -5.0f, -5.0f,
       5.0f, -5.0f, -5.0f,
       5.0f,  5.0f, -5.0f,
      -5.0f,  5.0f, -5.0f,

      -5.0f, -5.0f,  5.0f,
      -5.0f, -5.0f, -5.0f,
      -5.0f,  5.0f, -5.0f,
      -5.0f,  5.0f, -5.0f,
      -5.0f,  5.0f,  5.0f,
      -5.0f, -5.0f,  5.0f,

       5.0f, -5.0f, -5.0f,
       5.0f, -5.0f,  5.0f,
       5.0f,  5.0f,  5.0f,
       5.0f,  5.0f,  5.0f,
       5.0f,  5.0f, -5.0f,
       5.0f, -5.0f, -5.0f,

      -5.0f, -5.0f,  5.0f,
      -5.0f,  5.0f,  5.0f,
       5.0f,  5.0f,  5.0f,
       5.0f,  5.0f,  5.0f,
       5.0f, -5.0f,  5.0f,
      -5.0f, -5.0f,  5.0f,

      -5.0f,  5.0f, -5.0f,
       5.0f,  5.0f, -5.0f,
       5.0f,  5.0f,  5.0f,
       5.0f,  5.0f,  5.0f,
      -5.0f,  5.0f,  5.0f,
      -5.0f,  5.0f, -5.0f,

      -5.0f, -5.0f, -5.0f,
      -5.0f, -5.0f,  5.0f,
       5.0f, -5.0f, -5.0f,
       5.0f, -5.0f, -5.0f,
      -5.0f, -5.0f,  5.0f,
       5.0f, -5.0f,  5.0f
    };
    glUseProgram(skyboxShader);
    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    std::vector<std::string> faces;
    
    faces.push_back(std::string("./data/SkyBox/right.jpg"));
    faces.push_back(std::string("./data/SkyBox/left.jpg"));
    faces.push_back(std::string("./data/SkyBox/top.jpg"));
    faces.push_back(std::string("./data/SkyBox/bottom.jpg"));
    faces.push_back(std::string("./data/SkyBox/front.jpg"));
    faces.push_back(std::string("./data/SkyBox/back.jpg"));


    cubemapTexture = loadCubemap(faces);
	}

	void SkyMaps::render() {
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    glUseProgram(skyboxShader);
    glm::mat4 *ms_mult = multiply_stack(matrixStack);
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(*ms_mult));
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
	}

	// utility function for loading a 2D texture from file
	// ---------------------------------------------------
	GLuint SkyMaps::loadTexture(char const * path)
	{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
      GLenum format;
      if (nrComponents == 1)
        format = GL_RED;
      else if (nrComponents == 3)
      	format = GL_RGB;
    	else if (nrComponents == 4)
        format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
    }
    else
    {
    	std::cout << "Texture failed to load at path: " << path << std::endl;
      stbi_image_free(data);
    }

    return textureID;
	}

	// loads a cubemap texture from 6 individual texture faces
	// order:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front) 
	// -Z (back)
	// -------------------------------------------------------
	GLuint SkyMaps::loadCubemap(std::vector<std::string> faces)
	{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
      unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
      if (data)
      {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
      }
      else
      {
        std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
        stbi_image_free(data);
      }
    }
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	    return textureID;
	}

	SkyMaps::~SkyMaps() {
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVAO);
	}
}
