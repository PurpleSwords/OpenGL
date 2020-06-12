#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

//片段着色器
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int heigh)
{
	glViewport(0, 0, width, heigh);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

/*
	绘制多个物体：
	1、生成/配置所有的VAO、VBO和属性指针，并储存
	2、绘制时拿出相应的VAO并绑定，绘制结束后再解绑
*/
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "创建三角形", NULL, NULL);
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


	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
	};
	/*	
		顶点缓冲对象 Vertex Buffer Object,VBO
		顶点数组对象 Vertex Array Object,VAO

	*/
	unsigned int VBO, VAO;
	//glGenBuffers生成缓冲区对象名称，第一个参数表示生成个数
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	//1、绑定VAO
	glBindVertexArray(VAO);
	//2、复制顶点数组到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//把用户定义的数据复制到当前绑定缓冲的函数
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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


	//glfwWindowShouldClose检查GLFW是否被要求退出
	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		//检查输入
		processInput(window);

		//渲染指令，清空颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//4、渲染一个物体时要使用着色器程序：激活该对象，绘制物体
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);	//triangle,三角形

		//交换颜色缓冲(双缓冲)
		glfwSwapBuffers(window);
		//检查触发事件
		glfwPollEvents();
	}

	//释放资源
	glfwTerminate();
	//system("pause");
	return 0;
}