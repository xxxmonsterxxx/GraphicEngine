#pragma once

#ifndef OBJECT_H
#define OBJECT_H

class Scene;

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

#define FPS 60

typedef void(*callback)(int key, int scancode, int action, int mode, GLfloat time);
//typedef void(*behaviortemplate)(Object& obj, int key, int scancode, int action, int mode, GLfloat time);

const static GLfloat standard_vertices[20] = {
	// Позиции				// Текстурные координаты
	 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,    // Верхний правый
	 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,    // Нижний правый
	-0.5f, -0.5f, 0.0f,     0.0f, 0.0f,    // Нижний левый
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f     // Верхний левый
};

const static GLuint standard_indices[6] =
{
	0, 1, 2,
	0, 3, 2
};

class Object
{
	const char *name;
	const char *texture;

	GLfloat *vertices;

	GLuint VAO, VBO, EBO;

	////Size SETUP////
	glm::vec3 currsize = glm::vec3(1.f,1.f,1.f);
	glm::vec3 goalsize;
	GLfloat lastupdate_size = 0;
	GLfloat left_frame_size;
	////Size SETUP////

	////Move SETUP////
	glm::vec3 currPos = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat currAngle = 0.f;

	GLfloat left_frame_pos;
	glm::vec3 goalPos;
	GLfloat goalAngle;
	GLfloat lastupdate_pos = 0;
	////Move SETUP////

	int visible;

	////Sprite Sheet SETUP////
	GLfloat startSpriteX = 0;
	GLfloat startSpriteY = 0;
	GLboolean useSpriteSheet = GL_FALSE;
	GLint spriteSheetMultx = -1;
	GLfloat spriteSheetVectorx = 0;
	GLint spriteSheetMulty = -1;
	GLfloat spriteSheetVectory = 0;
	GLuint sprshtrow = 0;
	GLuint sprshtcolumn = 0;
	GLfloat animspeed = 20.f;
	GLfloat lastupdate_anim = 0;
	////Sprite Sheet SETUP////

	GLuint textureId;

public:
	GLboolean canStop = GL_FALSE;

	Scene *myScene;

	GLfloat * collision_box;

	Object(const char *name = "New object", const char *texture = "blu.png", GLboolean isSpriteSheet = GL_FALSE);
	Object(Object &obj);

	void behav(int key, int scancode, int action, int mode, GLfloat time);

	GLuint& getTextureId();
	const char* getTexture();

	void setupSpriteStartCoord(GLfloat x, GLfloat y);
	void setupSpriteSheet(GLfloat sprtshVX, GLfloat sprtshVY, GLuint rows, GLuint columns);
	glm::vec2 getNextSpriteCoord(GLfloat currtime);
	void setAnimSpeed(GLfloat speed);
	GLboolean isSpriteSheet();
	GLfloat getAnimSpeed();
	GLboolean needUpdate(GLfloat time);

	void getName();

	glm::vec3 getCurrentPosition();
	glm::vec3 getGoalPosition();

	void setCollisionBox(GLfloat dx, GLfloat dy);

	GLfloat *getVertices();
	void setTextCoord(glm::vec2 left_0, glm::vec2 left_1, glm::vec2 right_0, glm::vec2 right_1);

	void setSize(glm::vec3 goalsize, GLfloat deltatime);
	glm::vec3 getNextScale(GLfloat currtime);
	void setMove(glm::vec3 deltaPos, GLfloat deltaAngle, GLfloat deltatime);
	glm::mat4 getNextTransform(GLfloat time);
	void stopMove();

	GLuint& getVAO();
	GLuint& getEBO();
	GLuint& getVBO();

	int isVisible();
	void hide();
	void show();

	~Object();
};

#endif