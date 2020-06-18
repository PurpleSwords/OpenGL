#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "base.h"

int main_8()
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

	//����������ɫ��
	Shader ourShader("shader/shader_8.vs", "shader/shader_8.fs");

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

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();
		//����x��ƫ����
		float xOffset = 0.3f;
		ourShader.setFloat("xOffset", xOffset);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//��������
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//©����û�н���ɫ��ɾ��
	//glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}