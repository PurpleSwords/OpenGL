#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//程序ID
	unsigned int ID;

	//构造器读取并构建着色器
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		//1、从文件路径中获取顶点/片段着色器
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		/*
			get(1)  iostate exceptions() const;
			返回状态：goodbit,badbit,eofbit,failbit。
			set(2)  void exceptions(iostate except);
			The first form (1) returns the current exception mask for the stream.
			The second form (2) sets a new exception mask for the stream and 
			clears the stream’s error state flags (as if member clear() was called).
			流默认状态是goodbit，出错也不会主动抛异常
		*/
		//保证ifstream对象可以抛出异常
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//读取文件的缓冲内容到数据流中
			//rdbuf()可以实现一个流对象指向的内容用另一个流对象来输出
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//关闭文件处理器
			vShaderFile.close();
			fShaderFile.close();
			//转换数据流到string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR:SHADER:FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		//c_str()函数返回一个指向正规C字符串的指针常量, 内容与本string串相同
		////一定要使用strcpy()函数 等来操作方法c_str()返回的指针
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		/* 测试文件是否被正常读取
		std::cout << vShaderCode << std::endl;
		std::cout << fShaderCode << std::endl;
		*/
		//2、编译和链接着色器，检查错误函数进行封装
		unsigned int vertex, fragment;

		//顶点着色器
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "SHADER");
		//片段着色器
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		//着色器程序
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		//删除顶点/片段着色器
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	// 使用或激活程序
	void use()
	{
		glUseProgram(ID);
	}
	//uniform工具函数
	void setBool(const std::string &name, bool value) const	//const成员函数
	{
		//为当前程序对象指定Uniform变量的值
		//glGetUniformLocation返回一个整数，表示程序对象中特定统一变量的位置
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:
	void checkCompileErrors(GLuint shader, std::string type)
	{
		const int length = 1024;
		GLint success;
		GLchar infoLog[length];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, length, NULL, infoLog);
				std::cout << "ERROR:SHADER_COMPILE_FAILED of type: " << type << "\n" << infoLog << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, length, NULL, infoLog);
				std::cout << "ERROR:PROGRAM_LINK_FAILED of type: " << type << "\n" << infoLog << std::endl;
			}
		}
	}
};
