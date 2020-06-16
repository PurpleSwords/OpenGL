#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "base.h"


int main_2()
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
		//��һ��������
		-0.9f, -0.5f, 0.0f,  //����
		 0.0f, -0.5f, 0.0f,  //����
		-0.45f, 0.5f, 0.0f,  //���� 
		//��2��������
		 0.0f, -0.5f, 0.0f,  //����
		 0.9f, -0.5f, 0.0f,  //����
		 0.45f, 0.5f, 0.0f   //���� 
	};
	//����VAO��VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//�߿�ģʽ������鿴���������
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//ʹ��glDrawArrays���������˴�������������
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//GL_TRIANGLES��ÿ������֮����������Σ�֮�䲻����
		//GL_TRIANGLE_FAN����V0V1V2, V0V2V3, V0V3V4����������ʽ����������
		//GL_TRIANGLE_STRIP��˳����ÿ��������֮������������Ρ�����������Ա�֤����ͬ�ķ��������������ξ������ơ���V0V1V2, V1V2V3, V2V3V4��������ʽ����������
		glDrawArrays(GL_TRIANGLES, 0, 6);

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