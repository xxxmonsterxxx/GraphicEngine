#include "window.h"


Window& Window::Instance()
{
	static Window _instance;
	return _instance;
}

void Window::initWindow(GLuint width, GLuint height, const GLchar *windowName)
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	this->width = width;
	this->height = height;
	this->windowName = windowName;
	glwindow = glfwCreateWindow(width, height, windowName, nullptr, nullptr);
	if (glwindow == nullptr)
	{
		glfwTerminate();
	}
	glfwMakeContextCurrent(glwindow);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		//return -1;
	}
	int buffwidth, buffheight;
	glfwGetFramebufferSize(glwindow, &buffwidth, &buffheight);
	glViewport(0, 0, buffwidth, buffheight);

	shdMng = new ShaderManager();
	shaderProgram = shdMng->shaderProgram;
}

void Window::key_callback(int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(glwindow, GL_TRUE);

	for (int i = 0; i < scenes[currentScene].getNumberObjects(); i++)
		scenes[currentScene].getCurrObject().behav(key, scancode, action, mode, GLfloat(glfwGetTime()));

	//scenes[currentScene].getObject(i).executeBehaviorFunc(key, scancode, action, mode, GLfloat(glfwGetTime()));
}

Scene & Window::getCurrentScene()
{
	return scenes[currentScene];
}

GLFWwindow * Window::getWindow()
{
	return glwindow;
}

void Window::loadTexture(Scene &sc)
{
	for (int i = 0; i < sc.getNumberObjects(); i++)
	{
		loadTexture(sc.getObject(i));
	}
}

void Window::loadTexture(Object &obj)
{
	glGenTextures(1, &obj.getTextureId());
	glBindTexture(GL_TEXTURE_2D, obj.getTextureId());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int texwidth, texheight;
	unsigned char* image = SOIL_load_image(obj.getTexture(), &texwidth, &texheight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Window::setWindowBckgColor(float red, float green, float blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

void Window::show()
{
	for (int i = 0; i < currentNumberOfScenes; i++)
	{
		loadTexture(scenes[i]);
		initHelpEntities(scenes[i]);
	}

	while (!glfwWindowShouldClose(glwindow))
	{
		glfwPollEvents();
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		for (int i = 0; i < currentNumberOfScenes; i++)
			draw(scenes[i], GLfloat(glfwGetTime()));

		glfwSwapBuffers(glwindow);
	}
}

void Window::draw(Scene &sc, GLfloat currtime)
{
	for (int i = 0; i < sc.getNumberObjects(); i++)
	{
		draw(sc.getObject(i), currtime);
	}
}

void Window::draw(Object &obj, GLfloat currtime)
{
	glBindVertexArray(obj.getVAO());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, obj.getTextureId());
	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

	////Transform////
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(obj.getNextTransform(currtime)));
	////Transform////

	////Move on Sprite Sheet////
	GLuint multLoc = glGetUniformLocation(shaderProgram, "multx");
	glUniform1f(multLoc, obj.getNextSpriteCoord(currtime).x);
	multLoc = glGetUniformLocation(shaderProgram, "multy");
	glUniform1f(multLoc, obj.getNextSpriteCoord(currtime).y);
	////Move on Sprite Sheet////

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

int Window::addNewScene(Scene &newScene)
{
	if (currentNumberOfScenes < maxNumberOfScenes)
	{
		scenes[currentNumberOfScenes++] = newScene;
		return 1;
	}
	else
		return 0;
}

void Window::initHelpEntities(Object &obj)
{
	glGenBuffers(1, &obj.getVBO());
	glGenBuffers(1, &obj.getEBO());
	glGenVertexArrays(1, &obj.getVAO());

	glBindVertexArray(obj.getVAO());
	glBindBuffer(GL_ARRAY_BUFFER, obj.getVBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.getEBO());

	glBufferData(GL_ARRAY_BUFFER, sizeof(standard_vertices), obj.getVertices(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(standard_indices), standard_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); ////ÍÀÑÒÐÎÉÊÈ ÊÎÎÐÄÈÍÀÒ
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); ////ÍÀÑÒÐÎÉÊÈ ÖÂÅÒÀ
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Window::initHelpEntities(Scene &sc)
{
	for (int i = 0; i < sc.getNumberObjects(); i++)
		initHelpEntities(sc.getObject(i));
}