#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "base.h"

static const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;	//λ�ñ���������λ��Ϊ0\n"
	"layout (location = 1) in vec3 aColor;	//��ɫ����������λ��Ϊ1\n"
	"out vec3 ourColor;	//��Ƭ����ɫ�����һ����ɫ\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
	"	ourColor = aColor;	//��ourColor����Ϊ�Ӷ������ݵõ���������ɫ\n"
	"}\0";

static const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(ourColor, 1.0f);\n"
	"}\0";

int main_6()
{
	//��ʼ��
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(800, 600, "CreateTriangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);
	//ÿ�δ��ڴ�С������ʱ���ûص�����,�Դ�����е���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//������ɫ��
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER:VERTEX:COMPLETE_FAILED\n" << infoLog << std::endl;
	}
	//Ƭ����ɫ��
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER:FRAGMENT:COMPLETE_FAILED\n" << infoLog << std::endl;
	}
	//��ɫ������
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER:PROGRAM:LINK_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		//λ��				//��ɫ
		0.5f, -0.5f, 0.0f,  1.0f,0.0f,0.0f,	//����
		-0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,	//����
		0.0f, 0.5f, 0.0f,   0.0f,0.0f,1.0f 	//����
	};
	//����VAO��VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/*
		��ɫ����
		6��һ�飬Ҫ�޸Ĳ���
		���һ��������ʾ�����ڻ�������ʼλ�õ�ƫ����offset��ǰ3����λ�����ݣ�֮�������ɫ���ݣ���Ҫ�޸�ƫ����
	*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//�߿�ģʽ������鿴���������
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		//���ɲ�ɫ������
		//��դ��ʱ������Ƭ�β�ֵ���������λ�ð�������������˵����ɫ
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		//glGetUniformLocation��ѯuniform ourColor��λ��ֵ������-1��ʾû�в鵽
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		/*
			��ѯuniform��ַ��Ҫ��֮ǰʹ�ù���ɫ������
			���Ǹ���һ��uniform֮ǰ������ʹ�ó��򣨵���glUseProgram)��
			��Ϊ�����ڵ�ǰ�������ɫ������������uniform��
		*/
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//��������
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}