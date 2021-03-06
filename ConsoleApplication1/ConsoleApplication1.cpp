#include "pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define SCR_WIDTH  600
#define SCR_HEIGHT 600
using namespace std;

string vertexShader =
"#version 330 core"
"\n"
"layout(location = 0) in vec4 position;"
"\n"
"void main()\n"
"{\n"
"	gl_Position = position;\n"
"}\n";

string fragmentShader =
"#version 330 core"
"\n"
"layout(location = 0) out vec4 color;"
"\n"
"void main()\n"
"{\n"
"	color = vec4(0.0, 1.0, 0.0, 1.0);\n"
"}\n";


void processInput(GLFWwindow *window);

static int compileShader(unsigned int type, const string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, msg);
		cout << "Faild to Complie " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << endl;
		cout << msg << endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int createShader(const string &vertexShader, const string &fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;


}

float positions[8];
void drawQuad(float x , float y) {
	float l = 0.125;
	float temp[] = {
		-l + x,  l + y,
		 l + x,  l + y,
		 l + x, -l + y,
		-l + x, -l + y
	};
	for (int i = 0; i < 8; ++i)
		positions[i] = temp[i];
}
float x = 0.0, xSpeed = 0.005;
float y = 0.0, ySpeed = 0.005;
int main()
{
	// glfw: initialize and configure

	if(!glfwInit())
		return cout << "erre!" << endl , -1;
	
	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	if (glewInit() != GLEW_OK)
		cout << "Error: GLEW INIT\n";
	
	


	// creating a buffer and binding it to the buffer array
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions) * sizeof(float), positions, GL_STATIC_DRAW);// assigning the data to the buffer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int shader = createShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		drawQuad(x, y);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);

		glDrawArrays(GL_QUADS, 0, 4);
		cout << x << "   " << y << endl;
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cout << "Right Key s pressed and x =  "  << x << " " << endl;
		if(x + xSpeed < 1 - 0.125)
			x += xSpeed;

	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		cout << "Left Key is pressed" << endl;
		if (x - xSpeed > -1 + 0.125)
			x -= xSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		cout << "Up Key is pressed" << endl;
		if (y + ySpeed < 1 - 0.125)
			y += ySpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cout << "DOWN Key is pressed" << endl;
		if (y - ySpeed > -1 + 0.125)
			y -= ySpeed;
	}
	else {
		cout << "no key is pressed" << endl;
	}
}

