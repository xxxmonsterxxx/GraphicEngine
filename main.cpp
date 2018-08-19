#include <iostream>
#include "window.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Window &w = Window::Instance();
	w.key_callback(key, scancode, action, mode);
}

int main()
{
	Window &window = Window::Instance();
	window.initWindow();
	window.setWindowBckgColor(0.25f, 0.25f, 0.5f);
	glfwSetKeyCallback(window.getWindow(), key_callback);

	Object map("Map", "map.png", GL_FALSE);
	map.setSize(glm::vec3(2.f, 2.f, 1.f), 0.f);

	Object container("Container", "container.jpg", GL_FALSE);
	container.canStop = GL_TRUE;
	container.setSize(glm::vec3(0.5f, 0.5f, 1.f), 0.f);
	container.setMove(glm::vec3(0.5f, -0.57f, 0.f), 0.f, 0.f);

	Object man("Man","man.png",GL_FALSE);
	man.setupSpriteStartCoord(0.0, 0.0f);
	man.setMove(glm::vec3(0.f, -0.59f, 0.f), 0.f, 0.f);
	man.setTextCoord(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.25f), glm::vec2(0.1111f, 0.0f), glm::vec2(0.1111f, 0.25f));
	man.setSize(glm::vec3(0.5f, 0.5f, 1.f), 0.f);
	man.setAnimSpeed(0.5);

	Scene menu("Menu");
	menu.addObject(map);
	menu.addObject(man);
	menu.addObject(container);

	if (window.addNewScene(menu))
	{
		window.show();
	}

	return 0;
}