#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//����ID
	unsigned int ID;

	//��������ȡ��������ɫ��
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		//1�����ļ�·���л�ȡ����/Ƭ����ɫ��
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		/*
			get(1)  iostate exceptions() const;
			����״̬��goodbit,badbit,eofbit,failbit��
			set(2)  void exceptions(iostate except);
			The first form (1) returns the current exception mask for the stream.
			The second form (2) sets a new exception mask for the stream and 
			clears the stream��s error state flags (as if member clear() was called).
			��Ĭ��״̬��goodbit������Ҳ�����������쳣
		*/
		//��֤ifstream��������׳��쳣
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//��ȡ�ļ��Ļ������ݵ���������
			//rdbuf()����ʵ��һ��������ָ�����������һ�������������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//�ر��ļ�������
			vShaderFile.close();
			fShaderFile.close();
			//ת����������string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR:SHADER:FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		//c_str()��������һ��ָ������C�ַ�����ָ�볣��, �����뱾string����ͬ
		////һ��Ҫʹ��strcpy()���� ������������c_str()���ص�ָ��
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		/* �����ļ��Ƿ�������ȡ
		std::cout << vShaderCode << std::endl;
		std::cout << fShaderCode << std::endl;
		*/
		//2�������������ɫ���������������з�װ
		unsigned int vertex, fragment;

		//������ɫ��
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "SHADER");
		//Ƭ����ɫ��
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		//��ɫ������
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		//ɾ������/Ƭ����ɫ��
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	// ʹ�û򼤻����
	void use()
	{
		glUseProgram(ID);
	}
	//uniform���ߺ���
	void setBool(const std::string &name, bool value) const	//const��Ա����
	{
		//Ϊ��ǰ�������ָ��Uniform������ֵ
		//glGetUniformLocation����һ����������ʾ����������ض�ͳһ������λ��
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
