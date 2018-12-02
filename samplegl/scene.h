#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <AL/alut.h>
#include <ft2build.h>
#include FT_FREETYPE_H  
#define GLM_FORCE_SSE3
#define GLM_FORCE_SSSE3
#define GLM_FORCE_SSE41
#define GLM_FORCE_SSE42
#define GLM_FORCE_AVX
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "filesystem.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader_s.h"
#include "camera.h"
#include "model.h"

#include <iostream>

using namespace std;
using namespace glm;

class Scene {
private:
	Camera camera;
	bool IsEditor;
	int SCR_WIDTH, SCR_HEIGHT;
public:
	Scene(int SCR_WIDTH, int SCR_HEIGHT, const char * title,bool isEditor=true) {
		initglfw(SCR_WIDTH, SCR_HEIGHT, title);
		IsEditor = isEditor;
	}
	Scene() {

	}
	~Scene() {

	}
	int draw() {
	
	}
	GLFWwindow* initglfw(int scr_width, int scr_height,const char * title) {
		SCR_WIDTH = scr_width;
		SCR_HEIGHT = scr_height;
		// glfw: initialize and configure
	// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
		// glfw window creation
		// --------------------
		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
		return window;
	}
};

#endif