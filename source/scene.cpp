#include "scene.h"
#include <iostream>

Scene::Scene(const char *sceneName)
{
	this->sceneName = sceneName;
}

Scene::Scene(Scene &sc)
{
	this->sceneName = sc.sceneName;
	this->currentNumberOfObjects = sc.currentNumberOfObjects;
	for (int i = 0; i < currentNumberOfObjects; i++)
		this->objects[i] = sc.objects[i];
}

void Scene::hide()
{
	visible = 0;
}

void Scene::show()
{
	visible = 1;
}

int Scene::isVisible()
{
	return visible;
}

int Scene::addObject(Object &newObject)
{
	if (currentNumberOfObjects < maxNumberOfObjects)
	{
		newObject.myScene = this;
		objects[currentNumberOfObjects++] = newObject;
		return 1;
	}
	return 0;
}

Object& Scene::getObject(int ind)
{
	return objects[ind];
}

Object & Scene::getCurrObject()
{
	return objects[currentObj];
}

GLboolean Scene::canMoveObject(Object *obj)
{
	for (int i = 0; i < currentNumberOfObjects; i++)
	{
		if (objects[i].canStop)
		{
			//std::cout<<"Object "<<objects
			GLfloat x = obj->getCurrentPosition().x + 0.25f;
			std::cout << "Man coord x = " << obj->getCurrentPosition().x;
			std::cout << "\ncoord x = " << obj->getGoalPosition().x;
			GLfloat x_obj = objects[i].getCurrentPosition().x - 0.25f;
			if (x >= x_obj)
				return GL_FALSE;
		}
	}
	return GL_TRUE;
}

int Scene::getNumberObjects()
{
	return this->currentNumberOfObjects;
}

Scene::~Scene()
{

}