//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <AL/alut.h>
////#include <ft2build.h>
////#include FT_FREETYPE_H 
//#define GLM_FORCE_AVX
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include "filesystem.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#include "shader_s.h"
//#include "camera.h"
//#include "model.h"
#include <thread>
#include "scene.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void clShowfps(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1366;
const unsigned int SCR_HEIGHT = 768;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

ALCdevice *Device;
ALCcontext *Context;
ALuint buffer;
ALuint source;
ALCenum error;

void alErrorChk(const char * step) {
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		cout << step << " " << error << endl;
	}
}

int main()
{
	Scene scene;
	// Initialization
	GLFWwindow* window = scene.initglfw(SCR_WIDTH, SCR_HEIGHT, "LearnGL");
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	Context = alcCreateContext(Device, NULL);
	if (!alcMakeContextCurrent(Context))
		cout << "context error" << alGetError() << endl;

	Device = alcOpenDevice(NULL); // select the "preferred device"

	if (Device) {
		Context = alcCreateContext(Device, NULL);
		alcMakeContextCurrent(Context);
	}
	else {
		cout << "OpenAL init error." << endl;
	}

	alGenBuffers((ALuint)1, &buffer);
	// check for errors

	ALsizei size, freq;
	ALenum format;
	ALvoid *data;
	ALboolean loop = AL_FALSE;
	ALbyte * filename = (ALbyte*)("jurassicpark1.wav");
	//buffer = alutCreateBufferFromFile("jurassicpark1.wav");

	alutLoadWAVFile(filename, &format, &data, &size, &freq, &loop);
	alBufferData(buffer, format, data, size, freq);
	ALfloat listenerOri[] = { camera.Position.b, camera.Position.g, camera.Position.p, camera.Position.r, camera.Position.s, camera.Position.t };

	alListener3f(AL_POSITION, 0, 0, 1.0f);
	// check for errors
	alListener3f(AL_VELOCITY, 0, 0, 0);
	// check for errors
	alListenerfv(AL_ORIENTATION, listenerOri);
	// check for errors

	alGenSources((ALuint)1, &source);
	// check for errors

	alSourcef(source, AL_PITCH, 1);
	// check for errors
	alSourcef(source, AL_GAIN, 1);
	// check for errors
	alSource3f(source, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(source, AL_LOOPING, AL_TRUE);
	// check for errros

	//alutLoadWAVFile(&filename, &format, &data, &size, &freq, &loop);
	//alBufferData(buffer, format, data, size, freq);
	//alErrorChk("bufferData");
	// check for errors
	alSourcei(source, AL_BUFFER, buffer);
	// check for errors
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader ourShader("4.2.texture.vs", "4.2.texture.fs");

	// load models
	// -----------
	Model ourModel(FileSystem::getPath("house/WoodenCabinObj.obj"));
	//Model * Scene = new Model[]; 
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::vec3 lightPos(10.5f, 1.0f, 100.3f);

	alSourcePlay(source);
	// check for errors

	// render loop
	// -----------
	thread input= thread(processInput, window);
	thread fps = thread(clShowfps, window);
	input.detach();
	fps.detach();
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		
		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		ALfloat Ori[] = { camera.Position.b, camera.Position.g, camera.Position.p, camera.Position.r, camera.Position.s, camera.Position.t };
		for (unsigned short int i(0); i < 6; i++)
			listenerOri[i] = Ori[i];

		alListener3f(AL_POSITION, camera.Position.x, camera.Position.y, camera.Position.z);
		alListener3f(AL_VELOCITY, 0, 0, 0);
		alListenerfv(AL_ORIENTATION, listenerOri);

		// render the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setVec3("lightPos", lightPos);
		//ourShader.setVec3("lightPos", lightPos);

		// material properties
		ourShader.setFloat("shininess", 32.0f);
		
		ourModel.Draw(ourShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// cleanup context
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	Device = alcGetContextsDevice(Context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		Sleep(deltaTime*500);
	}
}
void clShowfps(GLFWwindow *window) {
	while (!glfwWindowShouldClose(window))
	{
		system("cls");
		cout <<"FPS: "<< 1 / deltaTime;
		Sleep(500);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}