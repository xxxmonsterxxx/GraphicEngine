#include <iostream>

class ShaderManager
{
	GLuint vertexShader;
	GLuint fragmentShader;

	const GLchar* vertexShaderSource =
		"#version 330 core\n"

		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec2 texCoord;\n"

		"out vec2 TexCoord;\n"
		"uniform mat4 transform;\n"

		"void main()\n"
		"{\n"
		"	gl_Position = transform * vec4(position, 1.0f);\n"
		"	TexCoord = texCoord;\n"
		"}\n\0";

	const GLchar* fragmentShaderSource =
		"#version 330 core\n"

		"in vec2 TexCoord;\n"

		"out vec4 fragColor;\n"

		"uniform sampler2D ourTexture;\n"
		"uniform float multx;\n"
		"uniform float multy;\n"

		"void main()\n"
		"{\n"
		"	vec4 color = texture(ourTexture, vec2(TexCoord.x + multx, 1.f - TexCoord.y - multy));\n"
		"	if (color.a < 0.1)\n"
		"		discard;\n"
		"	fragColor = color;\n"
		"}\n\0";

	void compileVertexShader();
	void compileFragmentShader();
	void compileShaderProgram();

public:
	ShaderManager() 
	{
		compileVertexShader();
		compileFragmentShader();
		compileShaderProgram();
	}

	GLuint shaderProgram;
};