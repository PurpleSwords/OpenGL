#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//������ɫ��
extern const char *vertexShaderSource;

//Ƭ����ɫ��
extern const char *fragmentShaderSource;

//�޸Ĵ����Сʱ�Ļص�
void framebuffer_size_callback(GLFWwindow* window, int width, int heigh);

//����������
void processInput(GLFWwindow* window);