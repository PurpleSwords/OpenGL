#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "base.h"

/*
	绘制多个物体：
	1、生成/配置所有的VAO、VBO和属性指针，并储存
	2、绘制时拿出相应的VAO并绑定，绘制结束后再解绑
*/
int main_1()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "CreateTriangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//设置当前窗口的上下文
	glfwMakeContextCurrent(window);

	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//0,0在窗口左下角（不是左上角）
	glViewport(0, 0, 800, 600);
	//每次窗口大小被调整时调用回调函数,对窗体进行调整
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//创建顶点着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//检测编译错误
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//无错误则success=1
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//片段着色器
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//检查错误
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//创建着色器程序
	unsigned int shaderProgram = glCreateProgram();
	//将之前编译的着色器附加到着色器程序对象上，并用glLinkProgram链接
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//检查链接是否成功,注意第二个参数是链接
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
	//链接成功后不再需要着色器对象，可以删除
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/*
	//分别表示xyz，范围-1到1
	float vertices[] = {
		//第一个三角形
		0.5f, 0.5f, 0.0f,	//右上角
		0.5f, -0.5f, 0.0f,	//右下角
		-0.5f, 0.5f, 0.0f,	//左上角
		//第二个三角形
		0.5f, -0.5f, 0.0f,	//右下角
		-0.5f, -0.5f, 0.0f,	//左下角
		-0.5f, 0.5f, 0.0f	//左上角
	};*/
	float vertices[] = {
		0.5f, 0.5f, 0.0f,	//右上角
		0.5f, -0.5f, 0.0f,	//右下角
		-0.5f, -0.5f, 0.0f,	//左下角
		-0.5f, 0.5f, 0.0f	//左上角
	};
	//索引绘制，绘制由两个三角形组合成的矩形时只定义4个顶点而不是6个顶点
	unsigned int indices[] = {	//索引从0开始
		0, 1, 3,	//第一个三角形
		1, 2, 3		//第二个三角形
	};
	/*	
		顶点缓冲对象 Vertex Buffer Object,VBO
		顶点数组对象 Vertex Array Object,VAO
		索引缓冲对象 Element/Index Buffer Object,EBO/IBO
	*/
	unsigned int VBO, VAO, EBO;
	//glGenBuffers生成缓冲区对象名称，第一个参数表示生成个数
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//1、绑定VAO
	glBindVertexArray(VAO);
	//2、复制顶点数组到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//把用户定义的数据复制到当前绑定缓冲的函数
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//3、设置顶点属性指针
	//如何解析顶点数据
	/*
		GLuint index, 指定要配置的顶点属性。之前在顶点着色器中使用了layout(location = 0)定义顶点属性的位置值，所以传0
		GLint size, 顶点属性的大小，vec3由3个值组成
		GLenum type, 指定数据的类型，vsc*都是由浮点数组成
		GLboolean normalized, 数据标准化：True则将所有数据映射到-1到1，无符号数据映射到0到1
		GLsizei stride, 步长设置，设置在连续的顶点属性组之间的间隔。顶点缓冲数据的存放形式类似于数组，连续存放
		const void *pointer,表示位置数据在缓冲中起始位置的偏移量offset，在开头则为0.注意参数类型。
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//通过绑定handle为0的VBO，可以取消对所有同类型VBO的绑定。
	//同一时间只能绑定一个同类型的VBO，只有当前被绑定的VBO才会被用户操作。
	//如果您使用的是现代OpenGL(核心配置文件),那么取消绑定VAO绝对是一种浪费(消耗性能)。
	//由于每个顶点的设置和绘制操作都必须绑定VAO,因此不需要解除之前的VAO绑定,再绑定不同的VAO。
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//线框模式
	//类似全局设置，之后的绘制都变成线框模式，需要恢复默认时可调用
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glfwWindowShouldClose检查GLFW是否被要求退出
	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		//检查输入
		processInput(window);

		//渲染指令，清空颜色缓冲
		//这里处理的是窗体背景色，不是三角形颜色，注意区分
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//4、渲染一个物体时要使用着色器程序：激活该对象，绘制物体
		glUseProgram(shaderProgram);
		//绘制三角形
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);	//triangle,三角形
		
		/*
			VAO同样可以保存EBO的绑定状态。
			VAO绑定时正在绑定的索引缓冲对象会被保存为VAO的元素缓存对象，绑定VAO的同时也会自动绑定EBO。
			当目标是GL_ELEMENT_ARRAY_BUFFER的时候，VAO会储存glBindBuffer的函数调用。
			这也意味着它也会储存解绑调用，所以不应在解绑VAO之前解绑索引数组缓冲，否则它就没有这个EBO配置了。
		*/
		//索引绘制矩形
		//glBindBuffer(GL_TRIANGLES, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);	//无需每次都解除绑定

		//交换颜色缓冲(双缓冲)
		glfwSwapBuffers(window);
		//检查触发事件
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	//释放资源
	glfwTerminate();
	//system("pause");
	return 0;
}