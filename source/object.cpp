#include "object.h"
#include "scene.h"
#include <iostream>

Object::Object(const char *name, const char *texture, GLboolean isSpriteSheet)
{
	this->name = name;
	this->texture = texture;
	this->useSpriteSheet = isSpriteSheet;

	vertices = new GLfloat[20];
	for (int i = 0; i < 20; i++)
		this->vertices[i] = standard_vertices[i];

	collision_box = new GLfloat[2];
	collision_box[0] = 0.5f;
	collision_box[1] = 0.5f;
}

Object::Object(Object &obj)
{
	this->currsize = obj.currsize;
	this->name = obj.name;
	this->texture = obj.texture;
	this->useSpriteSheet = obj.useSpriteSheet;

	vertices = new GLfloat[20];
	for (int i = 0; i < 20; i++)
		this->vertices[i] = obj.vertices[i];

	collision_box = new GLfloat[2];
	for (int i = 0; i < 2; i++)
		this->collision_box[i] = obj.collision_box[i];
}

void Object::setTextCoord(glm::vec2 left_0, glm::vec2 left_1, glm::vec2 right_0, glm::vec2 right_1)
{
	this->vertices[3] = right_1.x;
	this->vertices[4] = right_1.y;

	this->vertices[8] = right_0.x;
	this->vertices[9] = right_0.y;

	this->vertices[13] = left_0.x;
	this->vertices[14] = left_0.y;

	this->vertices[18] = left_1.x;
	this->vertices[19] = left_1.y;
}

GLfloat* Object::getVertices()
{
	return this->vertices;
}

void Object::setSize(glm::vec3 goalsize, GLfloat deltatime)
{
	if (deltatime != 0)
	{
		this->left_frame_size = deltatime * FPS;
		this->goalsize.x = currsize.x * goalsize.x;
		this->goalsize.y = currsize.y * goalsize.y;
	}
	else
	{
		this->currsize = goalsize;
	}
}

glm::vec3 Object::getNextScale(GLfloat currtime)
{
	if (left_frame_size >= 2)
	{
		if (currtime - lastupdate_size >= 1. / FPS)
		{
			glm::vec3 dSize = glm::vec3((goalsize.x - currsize.x) / left_frame_size,
				(goalsize.y - currsize.y) / left_frame_size, 0.f);
			currsize += dSize;

			left_frame_size--;
			lastupdate_size = currtime;
		}
	}

	return currsize;
}

glm::mat4 Object::getNextTransform(GLfloat currtime)
{
	if (left_frame_pos >= 1)
	{
		if (currtime - lastupdate_pos >= 1. / FPS)
		{
			glm::vec3 dVec = glm::vec3((goalPos.x - currPos.x) / left_frame_pos,
				(goalPos.y - currPos.y) / left_frame_pos, 0);
			currPos += dVec;

			GLfloat dAngle = (goalAngle - currAngle) / left_frame_pos;
			currAngle += dAngle;
			left_frame_pos--;
			lastupdate_pos = currtime;
		}
	}

	glm::mat4 transition;
	transition = glm::translate(transition, currPos);
	transition = glm::rotate(transition, currAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	transition = glm::scale(transition, getNextScale(currtime));

	return transition;
}

void Object::stopMove()
{
	this->left_frame_pos = 0;
}

GLuint& Object::getVAO()
{
	return this->VAO;
}

GLuint& Object::getEBO()
{
	return this->EBO;
}

GLuint& Object::getVBO()
{
	return this->VBO;
}

void Object::setMove(glm::vec3 deltaPos, GLfloat deltaAngle, GLfloat deltatime)
{
	if (deltatime != 0)
	{
		this->goalAngle = this->currAngle - deltaAngle;
		this->goalPos = this->currPos + deltaPos;
		this->left_frame_pos = deltatime * FPS;
	}
	else
	{
		this->currAngle += deltaAngle;
		this->currPos += deltaPos;
	}
}

void Object::behav(int key, int scancode, int action, int mode, GLfloat time)
{
	/*if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		this->useSpriteSheet = GL_TRUE;
		this->setupSpriteStartCoord(0.0f, 0.75f);
		this->setupSpriteSheet(0.1111f, 0.0f, 1, 9);
		this->setMove(glm::vec3(0.0f, 2.5f, 0.f), 0.f, 10.f);

	}
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
	{
		this->stopMove();
		this->useSpriteSheet = GL_FALSE;
		this->setupSpriteStartCoord(0.0f, 0.75f);
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		this->useSpriteSheet = GL_TRUE;
		this->setupSpriteStartCoord(0.0f, 0.25f);
		this->setupSpriteSheet(0.1111f, 0.0f, 1, 9);
		this->setMove(glm::vec3(0.0f, -2.5f, 0.f), 0.f, 10.f);
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
	{
		this->stopMove();
		this->useSpriteSheet = GL_FALSE;
		this->setupSpriteStartCoord(0.0f, 0.25f);
	}*/
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		this->useSpriteSheet = GL_TRUE;
		this->setupSpriteStartCoord(0.0f, 0.0f);
		this->setupSpriteSheet(0.1111f, 0.0f, 1, 9);
		this->setMove(glm::vec3(2.5f, 0.0f, 0.f), 0.f, 10.f);
		if (!this->myScene->canMoveObject(this))
		{
			std::cout << "STOP!\n";
			this->stopMove();
		}

	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		this->stopMove();
		this->useSpriteSheet = GL_FALSE;
		this->setupSpriteStartCoord(0.0f, 0.0f);
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		this->useSpriteSheet = GL_TRUE;
		this->setupSpriteStartCoord(0.0f, 0.5f);
		this->setupSpriteSheet(0.1111f, 0.0f, 1, 9);
		this->setMove(glm::vec3(-2.5f, 0.0f, 0.f), 0.f, 10.f);
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		this->stopMove();
		this->useSpriteSheet = GL_FALSE;
		this->setupSpriteStartCoord(0.0f, 0.5f);
	}
}

GLuint& Object::getTextureId()
{
	return textureId;
}

const char* Object::getTexture()
{
	return this->texture;
}

void Object::setupSpriteStartCoord(GLfloat x, GLfloat y)
{
	this->startSpriteX = x;
	this->startSpriteY = y;
}

void Object::setupSpriteSheet(GLfloat sprtshVX, GLfloat sprtshVY, GLuint rows, GLuint columns)
{
	this->spriteSheetVectorx = sprtshVX;
	this->spriteSheetVectory = sprtshVY;
	this->sprshtrow = rows;
	this->sprshtcolumn = columns;
}

glm::vec2 Object::getNextSpriteCoord(GLfloat currtime)
{
	if (useSpriteSheet)
	{
		if (this->needUpdate(currtime))
		{
			spriteSheetMultx = (++spriteSheetMultx) % sprshtcolumn;

			if (spriteSheetMultx == 0)
				spriteSheetMulty = (++spriteSheetMulty) % sprshtrow;

			glm::vec2 nextCoords(this->startSpriteX  + spriteSheetMultx * spriteSheetVectorx, this->startSpriteY + spriteSheetMulty * spriteSheetVectory);
			return nextCoords;
		}
		else
		{
			glm::vec2 nextCoords(this->startSpriteX + spriteSheetMultx * spriteSheetVectorx, this->startSpriteY + spriteSheetMulty * spriteSheetVectory);
			return nextCoords;
		}
	}
	else
	{
		glm::vec2 nextCoords(this->startSpriteX,this->startSpriteY);
		return nextCoords;
	}
}

void Object::setAnimSpeed(GLfloat speed)
{
	animspeed *= speed;
}

GLboolean Object::isSpriteSheet()
{
	return useSpriteSheet;
}

GLfloat Object::getAnimSpeed()
{
	return animspeed;
}

GLboolean Object::needUpdate(GLfloat time)
{
	GLfloat delta = time - lastupdate_anim;
	if (delta >= 1 / animspeed)
	{
		lastupdate_anim = time;
		return GL_TRUE;
	}
	else
		return GL_FALSE;
}

void Object::getName()
{
	int i = 0;
	while (name[i] != '\0')
	{
		std::cout << name[i];
		i++;
	}
}

glm::vec3 Object::getCurrentPosition()
{
	return this->currPos;
}

glm::vec3 Object::getGoalPosition()
{
	return this->goalPos;
}

void Object::setCollisionBox(GLfloat dx, GLfloat dy)
{
	this->collision_box[0] = dx;
	this->collision_box[1] = dy;
}

void Object::hide()
{
	visible = 0;
}

void Object::show()
{
	visible = 1;
}

int Object::isVisible()
{
	return this->visible;
}

Object::~Object()
{

}