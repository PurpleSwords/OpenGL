#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//顶点着色器
extern const char *vertexShaderSource;

//片段着色器
extern const char *fragmentShaderSource;

//修改窗体大小时的回调
void framebuffer_size_callback(GLFWwindow* window, int width, int heigh);

//处理按键输入
void processInput(GLFWwindow* window);