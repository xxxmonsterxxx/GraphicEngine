// GLFW

#ifndef WINDOW_H
#define WINDOW_H

#include "scene.h"

#include "shadermanager.h"


class Window
{
	static Window* _instance; //Singleton

	GLuint width, height;
	const GLchar *windowName;

	static const GLuint maxNumberOfScenes = 5;
	int currentNumberOfScenes = 0;
	int currentScene = 0;
	Scene scenes[maxNumberOfScenes];
	ShaderManager *shdMng;


	float red	= 1.f;
	float green = 1.f;
	float blue	= 1.f;


	GLFWwindow* glwindow;

	void initHelpEntities(Object &obj);
	void initHelpEntities(Scene &sc);

	//Help entities
	GLuint shaderProgram;


	void loadTexture(Scene &sc);
	void loadTexture(Object &obj);
	void draw(Scene &sc, GLfloat currtime);
	void draw(Object &obj, GLfloat currtime);

	Window() { ; }
	~Window() { ; }

	Window(Window const&) = delete;
	Window& operator= (Window const&) = delete;

public:

	static Window& Instance();

	void initWindow(GLuint width = 800, GLuint height = 600, const GLchar *windowName = "New window");

	void setWindowBckgColor(float red, float green, float blue);

	int addNewScene(Scene &newScene);

	void key_callback(int key, int scancode, int action, int mode);

	Scene& getCurrentScene();

	GLFWwindow* getWindow();

	void show();
};

#endif