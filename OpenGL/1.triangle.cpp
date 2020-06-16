#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "base.h"

/*
	���ƶ�����壺
	1������/�������е�VAO��VBO������ָ�룬������
	2������ʱ�ó���Ӧ��VAO���󶨣����ƽ������ٽ��
*/
int main_1()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(800, 600, "CreateTriangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//���õ�ǰ���ڵ�������
	glfwMakeContextCurrent(window);

	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//0,0�ڴ������½ǣ��������Ͻǣ�
	glViewport(0, 0, 800, 600);
	//ÿ�δ��ڴ�С������ʱ���ûص�����,�Դ�����е���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//����������ɫ��
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//���������
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//�޴�����success=1
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Ƭ����ɫ��
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//������ɫ������
	unsigned int shaderProgram = glCreateProgram();
	//��֮ǰ�������ɫ�����ӵ���ɫ����������ϣ�����glLinkProgram����
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//��������Ƿ�ɹ�,ע��ڶ�������������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
	//���ӳɹ�������Ҫ��ɫ�����󣬿���ɾ��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/*
	//�ֱ��ʾxyz����Χ-1��1
	float vertices[] = {
		//��һ��������
		0.5f, 0.5f, 0.0f,	//���Ͻ�
		0.5f, -0.5f, 0.0f,	//���½�
		-0.5f, 0.5f, 0.0f,	//���Ͻ�
		//�ڶ���������
		0.5f, -0.5f, 0.0f,	//���½�
		-0.5f, -0.5f, 0.0f,	//���½�
		-0.5f, 0.5f, 0.0f	//���Ͻ�
	};*/
	float vertices[] = {
		0.5f, 0.5f, 0.0f,	//���Ͻ�
		0.5f, -0.5f, 0.0f,	//���½�
		-0.5f, -0.5f, 0.0f,	//���½�
		-0.5f, 0.5f, 0.0f	//���Ͻ�
	};
	//�������ƣ�������������������ϳɵľ���ʱֻ����4�����������6������
	unsigned int indices[] = {	//������0��ʼ
		0, 1, 3,	//��һ��������
		1, 2, 3		//�ڶ���������
	};
	/*	
		���㻺����� Vertex Buffer Object,VBO
		����������� Vertex Array Object,VAO
		����������� Element/Index Buffer Object,EBO/IBO
	*/
	unsigned int VBO, VAO, EBO;
	//glGenBuffers���ɻ������������ƣ���һ��������ʾ���ɸ���
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//1����VAO
	glBindVertexArray(VAO);
	//2�����ƶ������鵽�����й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//���û���������ݸ��Ƶ���ǰ�󶨻���ĺ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//3�����ö�������ָ��
	//��ν�����������
	/*
		GLuint index, ָ��Ҫ���õĶ������ԡ�֮ǰ�ڶ�����ɫ����ʹ����layout(location = 0)���嶥�����Ե�λ��ֵ�����Դ�0
		GLint size, �������ԵĴ�С��vec3��3��ֵ���
		GLenum type, ָ�����ݵ����ͣ�vsc*�����ɸ��������
		GLboolean normalized, ���ݱ�׼����True����������ӳ�䵽-1��1���޷�������ӳ�䵽0��1
		GLsizei stride, �������ã������������Ķ���������֮��ļ�������㻺�����ݵĴ����ʽ���������飬�������
		const void *pointer,��ʾλ�������ڻ�������ʼλ�õ�ƫ����offset���ڿ�ͷ��Ϊ0.ע��������͡�
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//ͨ����handleΪ0��VBO������ȡ��������ͬ����VBO�İ󶨡�
	//ͬһʱ��ֻ�ܰ�һ��ͬ���͵�VBO��ֻ�е�ǰ���󶨵�VBO�Żᱻ�û�������
	//�����ʹ�õ����ִ�OpenGL(���������ļ�),��ôȡ����VAO������һ���˷�(��������)��
	//����ÿ����������úͻ��Ʋ����������VAO,��˲���Ҫ���֮ǰ��VAO��,�ٰ󶨲�ͬ��VAO��
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//�߿�ģʽ
	//����ȫ�����ã�֮��Ļ��ƶ�����߿�ģʽ����Ҫ�ָ�Ĭ��ʱ�ɵ���
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glfwWindowShouldClose���GLFW�Ƿ�Ҫ���˳�
	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		//�������
		processInput(window);

		//��Ⱦָ������ɫ����
		//���ﴦ����Ǵ��屳��ɫ��������������ɫ��ע������
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//4����Ⱦһ������ʱҪʹ����ɫ�����򣺼���ö��󣬻�������
		glUseProgram(shaderProgram);
		//����������
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);	//triangle,������
		
		/*
			VAOͬ�����Ա���EBO�İ�״̬��
			VAO��ʱ���ڰ󶨵������������ᱻ����ΪVAO��Ԫ�ػ�����󣬰�VAO��ͬʱҲ���Զ���EBO��
			��Ŀ����GL_ELEMENT_ARRAY_BUFFER��ʱ��VAO�ᴢ��glBindBuffer�ĺ������á�
			��Ҳ��ζ����Ҳ�ᴢ������ã����Բ�Ӧ�ڽ��VAO֮ǰ����������黺�壬��������û�����EBO�����ˡ�
		*/
		//�������ƾ���
		//glBindBuffer(GL_TRIANGLES, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);	//����ÿ�ζ������

		//������ɫ����(˫����)
		glfwSwapBuffers(window);
		//��鴥���¼�
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	//�ͷ���Դ
	glfwTerminate();
	//system("pause");
	return 0;
}