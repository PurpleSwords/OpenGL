#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "base.h"

int main_8()
{
	//初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "CreateTriangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);
	//每次窗口大小被调整时调用回调函数,对窗体进行调整
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//构建编译着色器
	Shader ourShader("shader/shader_8.vs", "shader/shader_8.fs");

	float vertices[] = {
		//位置				//颜色
		0.5f, -0.5f, 0.0f,  1.0f,0.0f,0.0f,	//右下
		-0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,	//左下
		0.0f, 0.5f, 0.0f,   0.0f,0.0f,1.0f 	//中上
	};
	//生成VAO、VBO
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
	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();
		//增加x轴偏移量
		float xOffset = 0.3f;
		ourShader.setFloat("xOffset", xOffset);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//结束处理
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//漏洞：没有将着色器删除
	//glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}